-module(b2bua_server).

-define(C_SERVER, {cnode_server, c_node()}).

-behaviour(gen_server).

-export([init/1, handle_call/3, handle_cast/2, handle_info/2, terminate/2, code_change/3]).
-export([start_link/0, stop/0]).

-export([process/0]).

-record(state, {c_port :: port(),
                c_node :: atom()}).

%%-------------------------------------------------------------------
%% PUBLIC API
%%-------------------------------------------------------------------
start_link() ->
    gen_server:start_link({local, ?MODULE}, ?MODULE, [], []).

process() ->
    ?C_SERVER ! {pid, self()},
    receive
        {pid, Pid} -> Pid
    after 5000 ->
        throw(timeout)
    end.

stop() -> ?C_SERVER ! stop.

init([]) ->
  %% We want to let the C/C++ node know who we are
  ThisNode = atom_to_list(node()),
  %% We build a fancy name for the C/C++ node
  CNode = c_node(),
  %% Finding the priv dir is a tricky thing, this function does it for us
  Priv = priv_dir(b2bua),
  Port =
    erlang:open_port({spawn_executable, Priv ++ "/b2bua"},
                     [{line,1000}, stderr_to_stdout,
                      {args, [
                              %% The command line arguments
                              %% including the node names and
                              %% the cookie
                              ThisNode, CNode,
                              erlang:get_cookie()]}]),
  %% Then we wait for a signal from the C/C++ side that everything is ready
  wait_for_ready(#state{c_port = Port, c_node = CNode}).

handle_info({nodedown, CNode}, State = #state{c_node = CNode}) ->
  error_logger:error_msg("C/C++ node is down!~n"),
  {stop, nodedown, State};
handle_info({Port, {data, {eol, "SEVERE: " ++ CLog}}}, State = #state{c_port = Port}) ->
  error_logger:warning_msg("C/C++ Error:\t~s~n", [CLog]),
  {noreply, State};
handle_info({Port, {data, {eol, "WARNING: " ++ CLog}}}, State = #state{c_port = Port}) ->
  error_logger:warning_msg("C/C++ Warning:\t~s~n", [CLog]),
  {noreply, State};
handle_info({Port, {data, {eol, "INFO | " ++ CLog}}}, State = #state{c_port = Port}) ->
  error_logger:info_msg("C/C++ Info:\t~s~n", [CLog]),
  {noreply, State};
handle_info({Port, {data, {eol, CLog}}}, State = #state{c_port = Port}) ->
  io:format("C/C++ Log:\t~s~n", [CLog]),
  {noreply, State};
handle_info({Port, {data, {noeol, CLog}}}, State = #state{c_port = Port}) ->
  error_logger:info_msg("C/C++ Log:\t~s...~n", [CLog]),
  {noreply, State};
handle_info({new_session, _SessionId, SipMessage}, State) ->
  error_logger:info_msg("Sip Message:\t~w...~n", [SipMessage]),
  {noreply, State};
handle_info(_Info, State) ->
  {noreply, State}.

handle_call(_Call, _From, State) -> {noreply, State}.
handle_cast(_Msg, State) -> {noreply, State}.
terminate(_Reason, State) -> catch erlang:port_close(State#state.c_port), ok.
code_change(_OldVersion, State, _Extra) -> {ok, State}.

%%-------------------------------------------------------------------
%% PRIVATE
%%-------------------------------------------------------------------
priv_dir(App) ->
  case code:priv_dir(App) of
    {error, bad_name} ->
      error_logger:info_msg("Couldn't find priv dir for the application, using ./priv~n"), "./priv";
    PrivDir -> filename:absname(PrivDir)
  end.

wait_for_ready(State = #state{c_port = Port}) ->
  receive
    {Port, {data, {eol, "READY"}}} ->
      error_logger:info_msg("C/C++ node started~n"),
      Process = process(),
      true = link(Process),
      error_logger:info_msg("Process ~p linked~n", [Process]),
      true = erlang:monitor_node(State#state.c_node, true),
      {ok, State};
    Info ->
      case handle_info(Info, State) of
        {noreply, NewState} ->
          wait_for_ready(NewState);
        {stop, Reason, _NewState} ->
          {stop, Reason}
      end
  end.

c_node() ->
    case string:tokens(atom_to_list(node()), "@") of
      [Name, Server] -> list_to_atom(Name ++ "_cnode@" ++ Server);
      _Node -> throw({bad_node_name, node()})
    end.
