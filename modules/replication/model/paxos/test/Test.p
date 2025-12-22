type tPaxosConfig = (number_proposers: int, number_acceptors: int, number_learners: int, broadcast_kind: tReliability);
event ePaxosConfig: (qourum: int);

fun SetupPaxos(config: tPaxosConfig) {
    var i: int;
    var proposers: set[Proposer];
    var jury: set[Acceptor];
    var school: set[Learner];
    var proposer_config: tProposerConfig;
    var acceptor_config: tAcceptorConfig;
    var learner_config: tLearnerConfig;

    announce ePaxosConfig, (qourum = config.number_acceptors / 2 + 1,);

    i = 0;
    while (i < config.number_acceptors) {
        acceptor_config = (broadcast_kind = config.broadcast_kind,);
        jury += (new Acceptor(acceptor_config));
        i = i + 1;
    }
    i = 0;
    while (i < config.number_learners) {
        learner_config = (broadcast_kind = config.broadcast_kind,);
        school += (new Learner(learner_config));
        i = i + 1;
    }
    i = 0;
    while (i < config.number_proposers) {
        i = i + 1;
        proposer_config = (broadcast_kind = config.broadcast_kind, jury = jury, school = school, value_to_propose = i + 100 + choose(50), id_proposer = i + choose(50));
        proposers += (new Proposer(proposer_config));
    }
}

machine BasicPaxos333Reliable {
    start state Init {
        entry {
            var config: tPaxosConfig;
            config = (number_proposers = 3, number_acceptors = 3, number_learners = 3, broadcast_kind = Reliable);
            SetupPaxos(config);
        }
    }
}

machine BasicPaxos333UnreliableMulti {
    start state Init {
        entry {
            var config: tPaxosConfig;
            config = (number_proposers = 3, number_acceptors = 3, number_learners = 3, broadcast_kind = Unreliable);
            SetupPaxos(config);
        }
    }
}

machine BasicPaxos333Unreliable {
    start state Init {
        entry {
            var config: tPaxosConfig;
            config = (number_proposers = 3, number_acceptors = 3, number_learners = 3, broadcast_kind = Unreliable);
            SetupPaxos(config);
        }
    }
}
machine BasicPaxos333ReliableMajority {
    start state Init {
        entry {
            var config: tPaxosConfig;
            config = (number_proposers = 3, number_acceptors = 3, number_learners = 3, broadcast_kind = Unreliable);
            SetupPaxos(config);
        }
    }
}

test testBasicPaxos333Reliable [main = BasicPaxos333Reliable]:
    assert OneValueTaught in (union Paxos, {BasicPaxos333Reliable});

test testBasicPaxos333ReliableMajority [main = BasicPaxos333ReliableMajority]:
    assert OneValueTaught in (union Paxos, {BasicPaxos333ReliableMajority});

test testBasicPaxos333Unreliable [main = BasicPaxos333Unreliable]:
    assert OneValueTaught in (union Paxos, {BasicPaxos333Unreliable});

test testBasicPaxos333UnreliableMulti [main = BasicPaxos333UnreliableMulti]:
    assert OneValueTaught in (union Paxos, {BasicPaxos333UnreliableMulti});
