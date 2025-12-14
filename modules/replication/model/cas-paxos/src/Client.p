machine Client {
    var value: int;
    var proposer: set[Proposer];
    var broadcast: tBroadcast;

    start state init {
        entry(setup: (broadcast: tBroadcast, proposer: Proposer)) {
            broadcast = setup.broadcast;
            proposer += (setup.proposer);
            goto Change;
        }
    }

    state Change {
        entry {
            Broadcast(broadcast, proposer, eChangeRequest, (client = this, value = value));
        }

        on eChangeResponse do (response: tChangeResponse) {

        }
    }
}

