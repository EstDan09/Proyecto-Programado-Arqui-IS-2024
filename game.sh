# Del old
if [ -f Outlaw ]; then
	rm Outlaw
fi

# Build new
gcc game.c -o Outlaw -Wno-unused -lncurses

# Run
./Outlaw

#chmod +x game.sh
#./game.sh