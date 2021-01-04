PROTOC ?= protoc
VERBOSE ?= 0

ifeq ($(VERBOSE),1)
  SILENCE =
else
  SILENCE = @
endif

##############################################################################################
# Default
##############################################################################################
all: cpp
.PHONY: all

##############################################################################################
# Help message
##############################################################################################
help:
	@echo "all        Generates everything"
	@echo "cpp        Generates CPP sources"
	@echo "clean      Cleanup"
	@echo ""
	@echo "Variables:"
	@echo "PROTOC     Path to protoc compiler"
	@echo "VERBOSE    Set to 1 for verbose output"
.PHONY:help

##############################################################################################
# CPP sources generation
##############################################################################################
cpp: cpp/embdb-layout.pb.h cpp/embdb-layout.pb.cc
.PHONY: cpp

cpp/embdb-layout.pb.h:
cpp/embdb-layout.pb.cc: embdb-layout.proto
	$(SILENCE)echo "Generating cpp sources ..."
	$(SILENCE)mkdir -p cpp
	$(SILENCE)$(PROTOC) --cpp_out=cpp $<

##############################################################################################
# Clean
##############################################################################################
clean:
	$(SILENCE)echo "Cleaning up!"
	$(SILENCE)$(RM) -r cpp
.PHONY: clean
