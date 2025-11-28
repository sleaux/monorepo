spec OneValueTaught observes eLearn {
    var value_learned: int;

    start state Init {
        entry {
            value_learned = -1;
        }

        on eLearn do (payload: tLearn) {
            assert(payload.value != -1);
            if (value_learned != -1) {
                assert(value_learned == payload.value);
            }
            value_learned = payload.value;
        }
    }
}
