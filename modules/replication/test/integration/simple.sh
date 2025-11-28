#!/bin/bash

"$TEST_SRCDIR/_main/modules/replication/src/server/server" --port 50051 &
"$TEST_SRCDIR/_main/modules/replication/src/server/server" --port 50052 &
"$TEST_SRCDIR/_main/modules/replication/src/server/server" --port 50053 &

"$TEST_SRCDIR/_main/modules/replication/src/client/client" --target localhost:50051 --value 2
"$TEST_SRCDIR/_main/modules/replication/src/client/client" --target localhost:50052 --value 2
"$TEST_SRCDIR/_main/modules/replication/src/client/client" --target localhost:50053 --value 3

"$TEST_SRCDIR/_main/modules/replication/src/client/client" --target localhost:50051
"$TEST_SRCDIR/_main/modules/replication/src/client/client" --target localhost:50052
"$TEST_SRCDIR/_main/modules/replication/src/client/client" --target localhost:50053
