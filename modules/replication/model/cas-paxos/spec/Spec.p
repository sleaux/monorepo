spec StoreWorks observes eStore {

    start state Init {
        entry {

        }

        on eStore do (payload: tStore) {
            assert(true);
        }
    }
}
