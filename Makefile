SUBDIRS =  debugfs hello_world list misc-char mm queue rwlock spinlock 	\
	   sysfs atomic percpu timers

all: subdirs

subdirs:
	for n in $(SUBDIRS); do $(MAKE) -C $$n || exit 1; done

clean:
	for n in $(SUBDIRS); do $(MAKE) -C $$n clean; \
	rm -f $$n/Module.symvers  $$n/Module.markers  $$n/modules.order; done
