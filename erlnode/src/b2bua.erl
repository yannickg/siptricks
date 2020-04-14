-module(b2bua).

-behaviour(application).

%% Application callbacks
-export([start/0, start/2, stop/1]).

%% ===================================================================
%% Application callbacks
%% ===================================================================

start() -> 
    application:start(b2bua).

start(_StartType, _StartArgs) ->
    error_logger:info_msg("Starting b2bua application...~n"),
    b2bua_sup:start_link().

stop(_State) ->
    ok.
