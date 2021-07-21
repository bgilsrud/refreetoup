
SOURCES = ft_test.c
MOBJS = $(SOURCES:%.c=meadecam_objs/%.o)
FOBJS = $(SOURCES:%.c=freetoup_objs/%.o)
CFLAGS = -I /usr/local/include/libmeadecam

FTLIBS = -L /usr/local/lib -lfreetoup
MCLIBS = -L /usr/local/lib -lmeadecam

all: ft_freetoup ft_meadecam

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

meadecam_objs/%.o: %.c
	mkdir -p $(@D)
	$(CC) -c $< -o $@ $(CFLAGS) -DBUILD_MEADECAM

freetoup_objs/%.o: %.c
	mkdir -p $(@D)
	$(CC) -c $< -o $@ $(CFLAGS) -DBUILD_FREETOUP

ft_freetoup: $(FOBJS)
	$(CC) -o $@ $< $(FTLIBS)

ft_meadecam: $(MOBJS)
	$(CC) -o $@ $< $(MCLIBS)
