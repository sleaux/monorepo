spec HistoryIsLinearizable observes eAcceptRequest, eAcceptResponse {
    var history: map[Acceptor, map[int, int]];

    start state Init {
        entry {
            history = default(map[Acceptor, map[int, int]]); 
        }

        on eAcceptRequest do (payload: tAcceptRequest) {
        
        }

        on eAcceptResponse do (payload: tAcceptResponse) {
            if (!(payload.acceptor in history)) {
                history[payload.acceptor] = default(map[int, int]);     
            }
            history[payload.acceptor][payload.timestamp] = payload.value;
            assert checkHistory(history);
        }           
    }

    fun checkHistory(history: map[Acceptor, map[int, int]]): bool {
        return true;
    }
}
