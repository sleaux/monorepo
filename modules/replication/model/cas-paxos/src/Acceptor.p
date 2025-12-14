machine Acceptor {
    var value: int;
    var timestamp: int;
    var promise: int;
    var broadcast: tBroadcast;

    start state Init {
        entry(broadcast: tBroadcast) {
            broadcast = broadcast;
            timestamp = 0;
            goto Accept;
        }
    }

    state Accept {
        on ePrepareRequest do (request: tPrepareRequest) {
            var confirm: bool;
            var proposer: set[machine];
            proposer += (request.proposer);
            confirm = request.timestamp > timestamp;
            timestamp = request.timestamp;
            send proposer[0], ePrepareResponse, (acceptor=this, timestamp=timestamp, confirmed=confirm);
            //Broadcast(broadcast, proposer, ePrepareResponse, (acceptor = this, timestamp = timestamp, confirmed = confirm));
        }

        on eAcceptRequest do (request: tAcceptRequest) {
            var proposer: set[machine];
            proposer += (request.proposer);
            promise = -1;
            value = request.value;
            send proposer[0], eAcceptResponse, (acceptor = this, timestamp = timestamp, value = value);
//            Broadcast(broadcast, proposer, eAcceptResponse, (acceptor = this, timestamp = timestamp, value = value));
        }
    }
}

