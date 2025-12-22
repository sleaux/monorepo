machine Basic {
    start state Init {
        entry {
            var proposer: Proposer;
            var acceptors: set[Acceptor];
            var clients: set[Client];
            acceptors += (new Acceptor((broadcast = Reliable, temp = 1)));
            proposer = new Proposer((broadcast = Reliable, acceptors = acceptors));
            clients += (new Client((broadcast = Reliable, proposer = proposer, value = 99)));
        }
    }
}

test testBasic [main = Basic]:
    assert HistoryIsLinearizable, ChangeSucceeded in (union CasPaxos, {Basic});
