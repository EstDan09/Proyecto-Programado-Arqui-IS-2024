# Del old
if [ -f Outlaw ]; then
	rm Outlaw
fi

# Build new
gcc outlaw.c -o Outlaw -Wno-unused -lncurses

# Run
./Outlaw

#chmod +x outlaw.sh
#./game.sh