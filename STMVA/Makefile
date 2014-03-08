CC=gcc
CXX=g++
RM=rm -f
CPPFLAGS=-g $(shell root-config --cflags)
LDFLAGS=-g $(shell root-config --ldflags)
LDLIBS=$(shell root-config --libs) -lTMVA

SRCS=STMVAConfig.cxx STMVA.cxx STMVAEvaluate.cxx STMVACreateMethodsTree.cxx
OBJS=$(subst .cxx,.o,$(SRCS))

%.o: %.cxx 
	$(CXX) $(CFLAGS) $(CPPFLAGS) -c $<

all: STMVA STMVAEvaluate STMVACreateMethodsTree

STMVA: $(OBJS)
	$(CXX) $(LDFLAGS) -o STMVA STMVA.o STMVAConfig.o $(LDLIBS)

STMVAEvaluate: $(OBJS)
	$(CXX) $(LDFLAGS) -o STMVAEvaluate STMVAEvaluate.o STMVAConfig.o $(LDLIBS)

STMVACreateMethodsTree: $(OBJS)
	$(CXX) $(LDFLAGS) -o STMVACreateMethodsTree STMVACreateMethodsTree.o STMVAConfig.o $(LDLIBS)

depend: .depend

.depend: $(SRCS)
	$(RM) ./.depend
	$(CXX) $(CPPFLAGS) -MM $^>>./.depend;

clean:
	$(RM) $(OBJS) STMVA STMVAEvaluate *~ .depend

#dist-clean: clean
#	$(RM) *~ .depend

include .depend
