machine Acceptor {
    var value: int;
    var timestamp: int;
    var promise: int;
    var broadcast: tReliability;

    start state Init {
        entry(setup: (broadcast: tReliability, temp: int)) {
            broadcast = setup.broadcast;
            timestamp = 0;
            goto Accept;
        }
    }

    state Accept {
        on ePrepareRequest do (request: tPrepareRequest) {
            var confirm: bool;
            confirm = request.timestamp > timestamp;
            timestamp = request.timestamp;
            Send(broadcast, request.proposer, ePrepareResponse, (acceptor = this, timestamp = timestamp, confirmed = confirm));
        }

        on eAcceptRequest do (request: tAcceptRequest) {
            promise = -1;
            value = request.value;
            Send(broadcast, request.proposer, eAcceptResponse, (acceptor = this, timestamp = timestamp, value = value));
        }
    }
}

