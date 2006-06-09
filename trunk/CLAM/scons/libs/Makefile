OPTIONS=prefix=/usr xmlbackend=xercesc release=yes with_osc_support=no

.PHONY: clean build all build-core build-processing build-audioio build-vmfl build-vmqt install install-core install-processing install-audioio install-vmfl install-vmqt

build: build-core build-processing build-audioio build-vmfl build-vmqt

clean:
	scons -c

build-core: 
	scons $(OPTIONS) core

build-processing:
	scons $(OPTIONS) processing

build-audioio:
	scons $(OPTIONS) audioio

build-vmfl:
	scons $(OPTIONS) vmfl

build-vmqt:
	scons $(OPTIONS) vmqt

install: install-core install-processing install-audioio install-vmfl install-vmqt

install-core:
	scons install_prefix=$(DESTDIR)/usr install_core
install-core-runtime:
	scons install_prefix=$(DESTDIR)/usr install_core_runtime
install-core-dev:
	scons install_prefix=$(DESTDIR)/usr install_core_dev

install-processing:
	scons install_prefix=$(DESTDIR)/usr install_processing
install-processing-runtime:
	scons install_prefix=$(DESTDIR)/usr install_processing_runtime
install-processing-dev:
	scons install_prefix=$(DESTDIR)/usr install_processing_dev

install-audioio:
	scons install_prefix=$(DESTDIR)/usr install_audioio
install-audioio-runtime:
	scons install_prefix=$(DESTDIR)/usr install_audioio_runtime
install-audioio-dev:
	scons install_prefix=$(DESTDIR)/usr install_audioio_dev

install-vmfl:
	scons install_prefix=$(DESTDIR)/usr install_vmfl
install-vmfl-runtime:
	scons install_prefix=$(DESTDIR)/usr install_vmfl_runtime
install-vmfl-dev:
	scons install_prefix=$(DESTDIR)/usr install_vmfl_dev

install-vmqt:
	scons install_prefix=$(DESTDIR)/usr install_vmqt
install-vmqt-runtime:
	scons install_prefix=$(DESTDIR)/usr install_vmqt_runtime
install-vmqt-dev:
	scons install_prefix=$(DESTDIR)/usr install_vmqt_dev
