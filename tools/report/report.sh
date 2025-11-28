SIZE_SERVER=$(ls -Lsh $SERVER | cut -d' ' -f1)
SIZE_CLIENT=$(ls -Lsh $CLIENT | cut -d' ' -f1)

echo \
	"SERVER ${SIZE_SERVER}
CLIENT ${SIZE_CLIENT}
"
