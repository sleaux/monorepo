machine Learner {
    var value_learned: tValue;
    var broadcast_kind: tReliability;

    start state Init {
        entry(config: tLearnerConfig) {
            value_learned = -1;
            broadcast_kind = config.broadcast_kind;
            goto Learn;
        }
    }

    state Learn {
        on eLearn do (payload: tLearn) {
            assert(payload.value != -1);
            assert((value_learned == -1) || (value_learned == payload.value));
            value_learned = payload.value;
        }
    }
}
