CFLAGS=-I ./ -Wall
LDFLAGS=-static

OBJS=main.o
BIN=imx8mn-gpiotoggle

default: $(BIN)
.PHONY: default clean

# Adapted from scottmcpeak.com/autodepend/autodepend.html
-include $(OBJS:.o=.d)
%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $*.o
	$(CC) -MM $(CFLAGS) $< > $*.d
	@cp -f $*.d $*.d.tmp
	@sed -e 's/.*://' -e 's/\\$$//' < $*.d.tmp | fmt -1 | \
	  sed -e 's/^ *//' -e 's/$$/:/' >> $*.d
	@rm -f $*.d.tmp

$(BIN):$(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDFLAGS)

clean:
	rm -f *.o *.d $(BIN)
