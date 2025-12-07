machine Client {
    var f: tChangeFunction;
    var proposer: set[Proposer];
    var broadcast: tBroadcast;

    start state init {
        entry(broadcast: tBroadcast) {
            broadcast = broadcast;
            goto Change;
        }
    }

    state Change {
        entry {
            Broadcast(broadcast, proposer, eChangeRequest, (client = this, f = f));
        }

        on eChangeResponse do (response: tChangeResponse) {

        }
    }
}

