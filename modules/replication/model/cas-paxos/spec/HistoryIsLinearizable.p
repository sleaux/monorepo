spec HistoryIsLinearizable observes eAcceptRequest {

    start state Init {
        entry {
            assert(true);
        }

        on eAcceptRequest do (payload: tAcceptRequest) {

        }
    }
}
