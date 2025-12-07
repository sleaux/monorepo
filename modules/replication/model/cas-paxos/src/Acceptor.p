machine Acceptor {
    var s: tState;
    var ballot_highest: tBallot;
    var promise: tBallot;
    var broadcast: tBroadcast;
    var proposer: set[Proposer];

    start state Init {
        entry(broadcast: tBroadcast) {
            broadcast = broadcast;
            ballot_highest = 0;
            goto Accept;
        }
    }

    state Accept {
        on ePrepareRequest do (request: tPrepareRequest) {
            var confirm: bool;
            confirm = request.ballot > ballot_highest;
            ballot_highest = request.ballot;
            proposer += (request.proposer);
            Broadcast(broadcast, proposer, ePrepareResponse, (acceptor = this, confirmed = confirm));
        }

        on eAcceptRequest do (request: tAcceptRequest) {
            promise = -1;
            s = request.new_state;
            Broadcast(broadcast, proposer, eAcceptResponse, (acceptor = this, confirmed = true));
        }
    }
}

