machine Proposer {
    var jury: set[Acceptor];
    var school: set[Learner];
    var number_ballot: tBallot;
    var number_highest_proposal: tBallot;
    var value_to_propose: tValue;
    var majority: int;
    var prepare_acks: set[Acceptor];
    var accept_acks: set[Acceptor];
    var broadcast_kind: tReliability;

    start state Init {
        entry(config : tProposerConfig) {
            broadcast_kind = config.broadcast_kind;
            jury = config.jury;
            school = config.school;
            number_ballot = config.id_proposer;
            value_to_propose = config.value_to_propose;
            majority = sizeof(jury) / 2 + 1;
            goto Prepare;
        }
    }

    state Prepare {
        entry {
            number_highest_proposal = -1;
            Broadcast(broadcast_kind, jury, ePrepareRequest, (proposer = this, number_ballot = number_ballot, value = value_to_propose));
        }

        on ePrepareResponse do (response: tPrepareResponse) {
            if (response.promised == number_ballot) {
                if (response.number_accepted > number_highest_proposal) {
                    number_highest_proposal = response.number_accepted;
                    value_to_propose = response.value_accepted;
                }
            }

            prepare_acks += (response.acceptor);

            if (sizeof(prepare_acks) >= majority) {
                goto Accept;
            }
        }
    }

    state Accept {
        entry {
            Broadcast(broadcast_kind, jury, eAcceptRequest, (proposer = this, number_ballot = number_ballot, value = value_to_propose));
        }

        on eAcceptResponse do (response: tAcceptResponse) {
            if (response.accepted == number_ballot) {
                accept_acks += (response.acceptor);
                if (sizeof(accept_acks) >= majority) {
                    goto Teach;
                }
            }
        }

        ignore ePrepareResponse;
    }

    state Teach {
        entry{
            Broadcast(broadcast_kind, school, eLearn, (ballot = number_ballot, value = value_to_propose));
        }

        ignore eAcceptResponse;
        ignore ePrepareResponse;
    }
}
