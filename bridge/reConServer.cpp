#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <os/OsIntTypes.h>

#include <recon/ReconSubsystem.hxx>
#include <recon/SipXHelper.hxx>

#include "ConversationManagerSettings.h"
#include "MyConversationManager.h"
#include "MyMessageDecorator.h"
#include "reConServerConfig.h"
#include "UserAgent.h"
#include "reConServer.h"

#include <rutil/Log.hxx>
#include <rutil/Logger.hxx>
#include <rutil/DnsUtil.hxx>

#define RESIPROCATE_SUBSYSTEM ReconSubsystem::RECON

NameAddr uri("sip:noreg@127.0.0.1");
bool autoAnswerEnabled = false;  // If enabled then reConServer will automatically answer incoming calls by adding to lowest numbered conversation
SharedPtr<ConversationProfile> conversationProfile;

reConServer::reConServer() :
   mConversationManager(NULL),
   mUserAgent(NULL)
{
}

reConServer::~reConServer()
{
   if (mConversationManager) delete mConversationManager;
   if (mUserAgent) delete mUserAgent;
}

void reConServer::startup(recon::ReConServerConfig& reConServerConfig)
{
   autoAnswerEnabled = reConServerConfig.getConfigBool("EnableAutoAnswer", false);
   bool registrationDisabled = reConServerConfig.getConfigBool("DisableRegistration", false);
   bool keepAlivesDisabled = reConServerConfig.getConfigBool("DisableKeepAlives", false);
   Data password = reConServerConfig.getConfigData("Password", "", true);
   Data dnsServers = reConServerConfig.getConfigData("DNSServers", "", true);;
   Data address = reConServerConfig.getConfigData("IPAddress", DnsUtil::getLocalIpAddress(), true);
   ConversationProfile::SecureMediaMode secureMediaMode = reConServerConfig.getConfigSecureMediaMode("SecureMediaMode", ConversationProfile::NoSecureMedia);
   bool secureMediaRequired = reConServerConfig.isSecureMediaModeRequired();
   ConversationProfile::NatTraversalMode natTraversalMode = reConServerConfig.getConfigNatTraversalMode("NatTraversalMode", ConversationProfile::NoNatTraversal);
   Data natTraversalServerHostname = reConServerConfig.getConfigData("NatTraversalServerHostname", "", true);
   unsigned short natTraversalServerPort = reConServerConfig.getConfigUnsignedShort("NatTraversalServerPort", 3478);
   Data stunUsername = reConServerConfig.getConfigData("StunUsername", "", true);
   Data stunPassword = reConServerConfig.getConfigData("StunPassword", "", true);
   unsigned short tcpPort = reConServerConfig.getConfigUnsignedShort("TCPPort", 5062);
   unsigned short udpPort = reConServerConfig.getConfigUnsignedShort("UDPPort", 5062);
   unsigned short tlsPort = reConServerConfig.getConfigUnsignedShort("TLSPort", 5063);
   unsigned short mediaPortStart = reConServerConfig.getConfigUnsignedShort("MediaPortStart", 17384);
   Data tlsDomain = reConServerConfig.getConfigData("TLSDomain", DnsUtil::getLocalHostName(), true);
   NameAddr outboundProxy = reConServerConfig.getConfigNameAddr("OutboundProxyUri", NameAddr(), true);
   uri = reConServerConfig.getConfigNameAddr("SIPUri", uri, true);
   Data loggingType = reConServerConfig.getConfigData("LoggingType", "cout", true);
   Data loggingLevel = reConServerConfig.getConfigData("LoggingLevel", "INFO", true);
   Data loggingFilename = reConServerConfig.getConfigData("LogFilename", "reConServer.log", true);
   unsigned int loggingFileMaxLineCount = reConServerConfig.getConfigUnsignedLong("LogFileMaxLines", 50000);
   bool localAudioEnabled = reConServerConfig.getConfigBool("EnableLocalAudio", false); // Defaults to false
   ConversationManager::MediaInterfaceMode mediaInterfaceMode = reConServerConfig.getConfigBool("GlobalMediaInterface", false)
      ? ConversationManager::sipXGlobalMediaInterfaceMode : ConversationManager::sipXConversationMediaInterfaceMode;
   unsigned int defaultSampleRate = reConServerConfig.getConfigUnsignedLong("DefaultSampleRate", 8000);
   unsigned int maximumSampleRate = reConServerConfig.getConfigUnsignedLong("MaximumSampleRate", 8000);
   bool enableG722 = reConServerConfig.getConfigBool("EnableG722", false);
   bool enableOpus = reConServerConfig.getConfigBool("EnableOpus", false);
   ReConServerConfig::Application application = reConServerConfig.getConfigApplication("Application", ReConServerConfig::None);

   resip::NameAddr contact  = reConServerConfig.getConfigNameAddr("Contact", NameAddr(), true);
   Data translationPattern = reConServerConfig.getConfigData("TranslationPattern", DnsUtil::getLocalHostName(), true);
   Data translationOutput = reConServerConfig.getConfigData("TranslationOutput", DnsUtil::getLocalHostName(), true);
   mAddressTranslator.addTranslation(translationPattern, translationOutput);

   std::vector<unsigned int> _codecIds;
   _codecIds.push_back(SdpCodec::SDP_CODEC_PCMU);           // 0 - pcmu
   _codecIds.push_back(SdpCodec::SDP_CODEC_PCMA);           // 8 - pcma
   _codecIds.push_back(SdpCodec::SDP_CODEC_SPEEX);          // 96 - speex NB 8,000bps
   _codecIds.push_back(SdpCodec::SDP_CODEC_SPEEX_15);       // 98 - speex NB 15,000bps
   _codecIds.push_back(SdpCodec::SDP_CODEC_SPEEX_24);       // 99 - speex NB 24,600bps
   _codecIds.push_back(SdpCodec::SDP_CODEC_L16_44100_MONO); // PCM 16 bit/sample 44100 samples/sec.
   _codecIds.push_back(SdpCodec::SDP_CODEC_ILBC);           // 108 - iLBC
   _codecIds.push_back(SdpCodec::SDP_CODEC_ILBC_20MS);      // 109 - Internet Low Bit Rate Codec, 20ms (RFC3951)
   _codecIds.push_back(SdpCodec::SDP_CODEC_SPEEX_5);        // 97 - speex NB 5,950bps
   _codecIds.push_back(SdpCodec::SDP_CODEC_GSM);            // 3 - GSM
   if(enableG722)
   {
      _codecIds.push_back(SdpCodec::SDP_CODEC_G722);        // 9 - G.722
   }
   if(enableOpus)
   {
      _codecIds.push_back(SdpCodec::SDP_CODEC_OPUS);        // Opus
   }
   _codecIds.push_back(SdpCodec::SDP_CODEC_TONES);          // 110 - telephone-event
   unsigned int *codecIds = &_codecIds[0];
   unsigned int numCodecIds = _codecIds.size();

   Log::initialize(loggingType, loggingLevel, "changeme", loggingFilename.c_str());
   GenericLogImpl::MaxLineCount = loggingFileMaxLineCount;

   // Setup logging for the sipX media stack
   // It is bridged to the reSIProcate logger
   SipXHelper::setupLoggingBridge("reConServer");
   //UserAgent::setLogLevel(Log::Warning, UserAgent::SubsystemAll);
   //UserAgent::setLogLevel(Log::Info, UserAgent::SubsystemRecon);
   
   initNetwork();
  
   InfoLog( << "reConServer settings:");
   InfoLog( << "  No Keepalives = " << (keepAlivesDisabled ? "true" : "false"));
   InfoLog( << "  Autoanswer = " << (autoAnswerEnabled ? "true" : "false"));
   InfoLog( << "  Do not register = " << (registrationDisabled ? "true" : "false"));
   InfoLog( << "  Local IP Address = " << address);
   InfoLog( << "  SIP URI = " << uri);
   InfoLog( << "  SIP Password = " << password);
   InfoLog( << "  Override DNS Servers = " << dnsServers);
   InfoLog( << "  Secure Media Mode = " << secureMediaMode);
   InfoLog( << "  NAT Traversal Mode = " << natTraversalMode);
   InfoLog( << "  NAT Server = " << natTraversalServerHostname << ":" << natTraversalServerPort);
   InfoLog( << "  STUN/TURN user = " << stunUsername);
   InfoLog( << "  STUN/TURN password = " << stunPassword);
   InfoLog( << "  TCP Port = " << tcpPort);
   InfoLog( << "  UDP Port = " << udpPort);
   InfoLog( << "  Media Port Range Start = " << mediaPortStart);
#ifdef USE_SSL
   InfoLog( << "  TLS Port = " << tlsPort);
   InfoLog( << "  TLS Domain = " << tlsDomain);
#endif
   InfoLog( << "  Outbound Proxy = " << outboundProxy);
   InfoLog( << "  Local Audio Enabled = " << (localAudioEnabled ? "true" : "false"));
   InfoLog( << "  Global Media Interface = " <<
      ((mediaInterfaceMode == ConversationManager::sipXGlobalMediaInterfaceMode) ? "true" : "false"));
   InfoLog( << "  Default sample rate = " << defaultSampleRate);
   InfoLog( << "  Maximum sample rate = " << maximumSampleRate);
   InfoLog( << "  Enable G.722 codec = " << (enableG722 ? "true" : "false"));
   InfoLog( << "  Enable Opus codec = " << (enableOpus ? "true" : "false"));
   InfoLog( << "  Log Type = " << loggingType);
   InfoLog( << "  Log Level = " << loggingLevel);
   InfoLog( << "  Log Filename = " << loggingFilename);
   InfoLog( << "type help or '?' for list of accepted commands." << std::endl);
 
   //////////////////////////////////////////////////////////////////////////////
   // Setup UserAgentMasterProfile
   //////////////////////////////////////////////////////////////////////////////
   
   SharedPtr<UserAgentMasterProfile> profile(new UserAgentMasterProfile);
   
   // Add transports
   if(udpPort)
   {
      profile->addTransport(UDP, udpPort, V4, address);
   }
   if(tcpPort)
   {
      profile->addTransport(TCP, tcpPort, V4, address);
   }
#ifdef USE_SSL
   if(tlsPort)
   {
      profile->addTransport(TLS, tlsPort, V4, address, tlsDomain);
   }
#endif
   
   // The following settings are used to avoid a kernel panic seen on an ARM embedded platform.
   // The kernel panic happens when either binding a udp socket to port 0 (OS selected),
   // or calling connect without first binding to a specific port.  There is code in the
   // resip transport selector that uses a utility UDP socket in order to determine
   // which interface should be used to route to a particular destination.  This code calls
   // connect with no bind.  By setting a fixed transport interface here that 
   // code will not be used.
   // The following line can be safely removed for other platforms
   //profile->setFixedTransportInterface(address);
   
   // Settings
   profile->setDefaultRegistrationTime(3600);
   profile->setDefaultFrom(uri);
   profile->setDigestCredential(uri.uri().host(), uri.uri().user(), password);

   // DNS Servers
   ParseBuffer pb(dnsServers);
   Data dnsServer;
   while(!dnsServers.empty() && !pb.eof())
   {
      pb.skipWhitespace();
      const char *start = pb.position();
      pb.skipToOneOf(ParseBuffer::Whitespace, ";,");  // allow white space 
      pb.data(dnsServer, start);
      if(DnsUtil::isIpV4Address(dnsServer))
      {
         InfoLog( << "Adding DNS Server: " << dnsServer);
         profile->addAdditionalDnsServer(dnsServer);
      }
      else
      {
         ErrLog( << "Tried to add dns server, but invalid format: " << dnsServer);
      }
      if(!pb.eof())
      {
         pb.skipChar();
      }
   }   

   // Disable Statisitics Manager
   profile->statisticsManagerEnabled() = false;
   
   // Add ENUM Suffixes from setting string - use code similar to dns server
   //profile->addEnumSuffix(enumSuffix);
   
   if(!keepAlivesDisabled)
   {
      profile->setKeepAliveTimeForDatagram(30);
      profile->setKeepAliveTimeForStream(180);
   }
   
   // Support Methods, etc.
   profile->validateContentEnabled() = false;
   profile->validateContentLanguageEnabled() = false;
   profile->validateAcceptEnabled() = false;

   profile->clearSupportedLanguages();
   profile->addSupportedLanguage(Token("en"));  

   profile->clearSupportedMimeTypes();
   profile->addSupportedMimeType(INVITE, Mime("application", "sdp"));
   profile->addSupportedMimeType(INVITE, Mime("multipart", "mixed"));  
   profile->addSupportedMimeType(INVITE, Mime("multipart", "signed"));  
   profile->addSupportedMimeType(INVITE, Mime("multipart", "alternative"));  
   profile->addSupportedMimeType(OPTIONS,Mime("application", "sdp"));
   profile->addSupportedMimeType(OPTIONS,Mime("multipart", "mixed"));  
   profile->addSupportedMimeType(OPTIONS, Mime("multipart", "signed"));  
   profile->addSupportedMimeType(OPTIONS, Mime("multipart", "alternative"));  
   profile->addSupportedMimeType(PRACK,  Mime("application", "sdp"));  
   profile->addSupportedMimeType(PRACK,  Mime("multipart", "mixed"));  
   profile->addSupportedMimeType(PRACK,  Mime("multipart", "signed"));  
   profile->addSupportedMimeType(PRACK,  Mime("multipart", "alternative"));  
   profile->addSupportedMimeType(UPDATE, Mime("application", "sdp"));  
   profile->addSupportedMimeType(UPDATE, Mime("multipart", "mixed"));  
   profile->addSupportedMimeType(UPDATE, Mime("multipart", "signed"));  
   profile->addSupportedMimeType(UPDATE, Mime("multipart", "alternative"));  
   profile->addSupportedMimeType(NOTIFY, Mime("message", "sipfrag"));  
   profile->addSupportedMimeType(INFO, Mime("application", "dtmf-relay"));

   profile->clearSupportedMethods();
   profile->addSupportedMethod(INVITE);
   profile->addSupportedMethod(ACK);
   profile->addSupportedMethod(CANCEL);
   profile->addSupportedMethod(OPTIONS);
   profile->addSupportedMethod(BYE);
   profile->addSupportedMethod(REFER);    
   profile->addSupportedMethod(NOTIFY);    
   profile->addSupportedMethod(SUBSCRIBE); 
   profile->addSupportedMethod(UPDATE);    
   profile->addSupportedMethod(PRACK);     
   profile->addSupportedMethod(INFO);    
   //profile->addSupportedMethod(MESSAGE);

   profile->clearSupportedOptionTags();
   profile->addSupportedOptionTag(Token(Symbols::Replaces));      
   profile->addSupportedOptionTag(Token(Symbols::Timer)); 
   profile->addSupportedOptionTag(Token(Symbols::NoReferSub));
   profile->addSupportedOptionTag(Token(Symbols::AnswerMode));
   profile->addSupportedOptionTag(Token(Symbols::TargetDialog));
   //profile->addSupportedOptionTag(Token(Symbols::C100rel));  // Automatically added by calling setUacReliableProvisionalMode

   profile->setUacReliableProvisionalMode(MasterProfile::Supported);

   profile->clearSupportedSchemes();
   profile->addSupportedScheme("sip");  
#ifdef USE_SSL
   profile->addSupportedScheme("sips");
#endif

   // Have stack add Allow/Supported/Accept headers to INVITE dialog establishment messages
   profile->clearAdvertisedCapabilities(); // Remove Profile Defaults, then add our preferences
   profile->addAdvertisedCapability(Headers::Allow);  
   //profile->addAdvertisedCapability(Headers::AcceptEncoding);  // This can be misleading - it might specify what is expected in response
   profile->addAdvertisedCapability(Headers::AcceptLanguage);  
   profile->addAdvertisedCapability(Headers::Supported);  
   profile->setMethodsParamEnabled(true);

   profile->setOverrideHostAndPort(contact.uri());
   if(!outboundProxy.uri().host().empty())
   {
      profile->setOutboundProxy(outboundProxy.uri());
   }
   
   profile->setUserAgent("ConversationManager/reConServer");
   profile->rtpPortRangeMin() = mediaPortStart;
   profile->rtpPortRangeMax() = mediaPortStart + 101; // Allows 100 media streams
   
   if(natTraversalMode == ConversationProfile::NoNatTraversal)
   {
      StackLog(<<"NAT traversal features not enabled, "
                 "adding message decorator for SDP connection address");
      SharedPtr<MessageDecorator> md(new MyMessageDecorator());
      profile->setOutboundDecorator(md);
   }

   //////////////////////////////////////////////////////////////////////////////
   // Setup ConversationProfile
   //////////////////////////////////////////////////////////////////////////////
  
   conversationProfile = SharedPtr<ConversationProfile>(new ConversationProfile(profile));
   if(uri.uri().user() != "noreg" && !registrationDisabled)
   {
      conversationProfile->setDefaultRegistrationTime(3600);
   }
   else
   {
      conversationProfile->setDefaultRegistrationTime(0);
   }
   conversationProfile->setDefaultRegistrationRetryTime(120);  // 2 mins
   conversationProfile->setDefaultFrom(uri);
   conversationProfile->setDigestCredential(uri.uri().host(), uri.uri().user(), password);

   // Setup NatTraversal Settings
   conversationProfile->natTraversalMode() = natTraversalMode;
   conversationProfile->natTraversalServerHostname() = natTraversalServerHostname;
   conversationProfile->natTraversalServerPort() = natTraversalServerPort;
   conversationProfile->stunUsername() = stunUsername;
   conversationProfile->stunPassword() = stunPassword;

   // Secure Media Settings
   conversationProfile->secureMediaMode() = secureMediaMode;
   conversationProfile->secureMediaRequired() = secureMediaRequired;
   conversationProfile->secureMediaDefaultCryptoSuite() = ConversationProfile::SRTP_AES_CM_128_HMAC_SHA1_80;
   
   ConversationManagerSettings settings;
   mConversationManager = new recon::MyConversationManager(settings, *this);
   mUserAgent = new MyUserAgent(mConversationManager, profile);
   
   mConversationManager->buildSessionCapabilities(address, numCodecIds, codecIds, conversationProfile->sessionCaps());
   mUserAgent->addConversationProfile(conversationProfile);

   mUserAgent->startup();

   //ua.createSubscription("message-summary", uri, 120, Mime("application", "simple-message-summary")); // thread safe
}

void reConServer::process(int timeoutMs)
{
   mUserAgent->process(timeoutMs);
}

void reConServer::shutdown()
{
   DebugLog( << "reConServer::shutdown()");
   mUserAgent->shutdown();
}

resip::Data reConServer::createConversation(const resip::Data& caller, const resip::Data& callee, bool recordConversation /* = true */)
{
   DebugLog( << "reConServer::createConversation() - Creating conversation: " << caller << " to " << callee << " - recording: " << (recordConversation? "true": "false"));
   ConversationHandle convHandle = mConversationManager->createConversation();
   DebugLog( << "Created conversation " << convHandle);

   prepareConversation(convHandle, caller, callee, recordConversation);
}

void reConServer::dropVoiceMail(const resip::Data& conversationId)
{
}

bool reConServer::translateAddress(const resip::Data& address, resip::Data& translation, bool failIfNoRule /* = false */)
{
   return mAddressTranslator.translate(address, translation, failIfNoRule);
}

void reConServer::onConversationDestroyed(ConversationHandle convHandle)
{
   DebugLog( << "reConServer::onConversationDestroyed: " << convHandle);
}

/** 
  Notifies an application when a Participant has been destroyed.  This is 
  useful for tracking when audio playback via MediaResourceParticipants has 
  stopped.

  @param partHandle Handle of the destroyed participant
 */
void reConServer::onParticipantDestroyed(ParticipantHandle partHandle)
{
   DebugLog( << "reConServer::onParticipantDestroyed: " << partHandle);
}

void reConServer::onDtmfEvent(ParticipantHandle partHandle, int dtmf, int duration, bool up)
{
   DebugLog( << "reConServer::onDtmfEvent: " << partHandle);
}

/**
  Notifies an application about a new remote participant that is attempting
  to contact it.

  @param partHandle Handle of the new incoming participant
  @param msg Includes information about the caller such as name and address
  @param autoAnswer Set to true if auto answer has been requested
 */
void reConServer::onIncomingParticipant(ParticipantHandle partHandle, const SipMessage& msg, bool autoAnswer, ConversationProfile& conversationProfile)
{
   DebugLog( << "reConServer::onIncomingParticipant: " << partHandle);
   // this only gets called when participants are calling in
}

void reConServer::onRequestOutgoingParticipant(ParticipantHandle partHandle, const SipMessage& msg, ConversationProfile& conversationProfile)
{
   DebugLog( << "reConServer::onRequestOutgoingParticipant: " << partHandle);
}

/**
  Notifies an application about a disconnect by a remote participant.  
  For SIP this could be a BYE or a CANCEL request.

  @param partHandle Handle of the participant that terminated
  @param statusCode The status Code for the termination.
 */
void reConServer::onParticipantTerminated(ParticipantHandle partHandle, unsigned int statusCode)
{
   DebugLog( << "reConServer::onParticipantTerminated: " << partHandle);

   bridge::Conversation::Participant* part = NULL;
   bridge::Conversation* conv = conversationFromParticipanHandle(partHandle, part);
   if (conv)
   {
      if (part)
      {
         if (part->isConversationOwner())
         {
            // TODO: do we force drop this conversation?
         }
      
         // remove participant who just dropped
         //mConversationManager->destroyParticipant(part->handle());
         part->removeFromConversation();

         if (conv->canDeleteConversation())
         {
            DebugLog( << "Deleting all participants in conversation: " << conv->handle());

            // TODO: make this its own function
            conv->deleteAllParticipants(*mConversationManager);
            mConversationManager->destroyConversation(conv->handle());
            bridge::Conversation::Map::iterator iter = mConversations.find(conv->uuid());
            if (iter != mConversations.end())
            {
               mConversations.erase(iter);
            }
         }
      }
   }
   else
   {
      InfoLog( << "Error - Participant " << partHandle << " not found");
   }
}

void reConServer::onParticipantProceeding(ParticipantHandle partHandle, const SipMessage& msg)
{
   DebugLog( << "reConServer::onParticipantProceeding: " << partHandle);
}

void reConServer::onRelatedConversation(ConversationHandle relatedConvHandle, ParticipantHandle relatedPartHandle, 
                                        ConversationHandle origConvHandle, ParticipantHandle origPartHandle)
{
   DebugLog( << "reConServer::onRelatedConversation()");
}

void reConServer::onParticipantAlerting(ParticipantHandle partHandle, const SipMessage& msg)
{
   DebugLog( << "reConServer::onParticipantAlerting: " << partHandle);
   // We've received 18x provisional
}

/**
  Notifies an application that a remote participant call attempt is 
  now connected.

  @param partHandle Handle of the participant that is connected
  @param msg SIP message that caused the connection
 */
void reConServer::onParticipantConnected(ParticipantHandle partHandle, const SipMessage& msg)
{
   DebugLog( << "reConServer::onParticipantConnected: " << partHandle);

   bridge::Conversation::Participant* part = NULL;
   bridge::Conversation* conv = conversationFromParticipanHandle(partHandle, part);
   if (conv)
   {
      if (part)
      {
         if (part->isConversationOwner())
         {
            // TODO
            //Uri url("file:/home/yannickg/greetings/eng/redirecting.wav");
            //ParticipantHandle mediaHandle = mConversationManager->createMediaResourceParticipant(conv->handle(), url);

            Uri url("tone:ringback");
            ParticipantHandle ringbackHandle = mConversationManager->createMediaResourceParticipant(conv->handle(), url);
            conv->setMediaParticipant(ringbackHandle);

            // need to invite second participant here
            bridge::Conversation::Participant* newPart = conv->getNextParticipantToInvite();
            if (newPart)
            {
               DebugLog( << "Inviting next participant: " << newPart->address());
               NameAddr dest(newPart->address());
               newPart->handle() = mConversationManager->createRemoteParticipant(conv->handle(), dest);
            }
         }
         else
         {
            // stop ringback tone
            if (conv->mediaParticipant())
            {
               DebugLog( << "Deleting media participant (rinback tone): " << conv->mediaParticipant()->handle());
               mConversationManager->destroyParticipant(conv->mediaParticipant()->handle());
               conv->mediaParticipant()->removeFromConversation();
            }
         
            if (conv->recordConversation())
            {
               DebugLog( << "Creating recording participant");
               ParticipantHandle recordingHandle = mConversationManager->recordConversation(conv->handle(), "/home/yannickg/recordings/sessonxyz.wav");
               conv->setRecordingParticipant(recordingHandle);
            }
         }
      }
   }
   else
   {
      InfoLog( << "Error - Participant " << partHandle << " not found");
   }
}

void reConServer::onParticipantRedirectSuccess(ParticipantHandle partHandle)
{
   DebugLog( << "reConServer::onParticipantRedirectSuccess: " << partHandle);
}

void reConServer::onParticipantRedirectFailure(ParticipantHandle partHandle, unsigned int statusCode)
{
   DebugLog( << "reConServer::onParticipantRedirectFailure: " << partHandle);
}

void reConServer::prepareConversation(recon::ConversationHandle convHandle, const resip::Data& caller, const resip::Data& callee, bool recordConversation)
{
   DebugLog( << "reConServer::prepareConversation() - Preparing conversation: " << caller << " to " << callee);

   bridge::Conversation conv(convHandle, recordConversation);

   NameAddr dest(caller);
   ParticipantHandle partHandle = mConversationManager->createRemoteParticipant(convHandle, dest);

   mConversations[conv.uuid()] = conv;

   // Note: the following HAS to be AFTER what's above
   bridge::Conversation::Map::iterator iter = mConversations.find(conv.uuid());
   if (iter != mConversations.end())
   {
      iter->second.setConversationOwner(caller, partHandle);
      iter->second.addNextParticipant(callee);
   }
}

bridge::Conversation* reConServer::conversationFromParticipanHandle(recon::ParticipantHandle partHandle, bridge::Conversation::Participant*& part)
{
   DebugLog( << "reConServer::conversationFromParticipanHandle: " << partHandle);

   bridge::Conversation* conv = NULL;
   bridge::Conversation::Map::iterator iter = mConversations.begin();
   for (; iter != mConversations.end(); ++iter)
   {
      if (iter->second.hasThisParticipant(partHandle, part))
      {
         conv = &iter->second;
         break;
      }
   }
   return conv;
}

/* ====================================================================

 Copyright (c) 2007-2008, Plantronics, Inc.
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are 
 met:

 1. Redistributions of source code must retain the above copyright 
    notice, this list of conditions and the following disclaimer. 

 2. Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution. 

 3. Neither the name of Plantronics nor the names of its contributors 
    may be used to endorse or promote products derived from this 
    software without specific prior written permission. 

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
 "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
 LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR 
 A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
 OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
 LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
 DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY 
 THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
 OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 ==================================================================== */
