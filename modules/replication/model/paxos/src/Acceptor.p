machine Acceptor {
    var number_prepared: tBallot;
    var value_accepted: tValue;
    var number_accepted: tBallot;
    var broadcast_kind: tBroadcast;

    start state Init {
        entry (config: tAcceptorConfig){
            number_prepared = -1;
            value_accepted = -1;
            number_accepted = -1;
            broadcast_kind = config.broadcast_kind;
            goto Accept;
        }
    }

    state Accept {
        on ePrepareRequest do (request: tPrepareRequest) {
            var proposer: set[machine]; // set is not ideal, not sure how to coerce a single value into a set.
            if (request.number_ballot > number_prepared) {
                proposer += (request.proposer);
                Broadcast(broadcast_kind, proposer, ePrepareResponse, (acceptor = this, promised = request.number_ballot, value_accepted = value_accepted, number_accepted = number_accepted));
                number_prepared = request.number_ballot;
            }
        }

        on eAcceptRequest do (request: tAcceptRequest) {
            var proposer: set[machine]; // set is not ideal, not sure how to coerce a single value into a set.
            if (request.number_ballot >= number_prepared) {
                value_accepted = request.value;
                number_accepted = request.number_ballot;
                number_prepared = request.number_ballot;
                proposer += (request.proposer);
                Broadcast(broadcast_kind, proposer, eAcceptResponse, (acceptor = this, accepted = request.number_ballot));
            }
        }
    }
}
