machine Proposer {
    var ballot: tBallot;
    var ballot_highest: tBallot;
    var f: tChangeFunction;
    var s: tState;

    var broadcast: tBroadcast;
    var acceptors: set[Acceptor];
    var accept_acks: set[Acceptor];
    var prepare_acks: set[Acceptor];

    var client: set[Client];

    start state Init {
        entry(broadcast: tBroadcast) {
            broadcast = broadcast;
            ballot_highest = 0;
            goto Wait;
        }
   }

    state Wait {
        entry {
            prepare_acks = default(set[Acceptor]);
            accept_acks = default(set[Acceptor]);
        }

        on eChangeRequest do (request: tChangeRequest) {
            f = request.f;
            client += (request.client);
            goto Prepare; 
        }
       

    }

    state Prepare {
        entry {
            ballot = ballot_highest + 1;
            ballot_highest = ballot;
            Broadcast(broadcast, acceptors, ePrepareRequest, (proposer = this, ballot = ballot));
        }

        on ePrepareResponse do (response: tPrepareResponse) {
            if (response.confirmed) {
                prepare_acks += (response.acceptor);
                if (sizeof(prepare_acks) >= sizeof(acceptors) / 2 + 1) {
                    goto Accept;
                }
            } else {
                // Tell client it failed by returning a non-modified state.
                Broadcast(broadcast, client, eChangeResponse, (proposer = this, value = s));
                goto Wait;
            }
        }
    }

    state Accept {
        entry {
            // Apply f to state
            s = f;
            Broadcast(broadcast, acceptors, eAcceptRequest, (proposer = this, ballot = ballot, new_state = s));
        }

        on eAcceptResponse do (response: tAcceptResponse) {
            if (response.confirmed) {
                accept_acks += (response.acceptor);
                if (sizeof(accept_acks) >= sizeof(acceptors) / 2 + 1) {
                    Broadcast(broadcast, client, eChangeResponse, (proposer = this, value = s));
                    goto Wait;
                }
            }
        }
    }
}

