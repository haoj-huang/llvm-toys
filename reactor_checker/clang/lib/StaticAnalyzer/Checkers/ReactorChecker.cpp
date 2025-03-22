
#include "clang/StaticAnalyzer/Core/BugReporter/BugType.h"
#include "clang/StaticAnalyzer/Core/Checker.h"
#include "clang/StaticAnalyzer/Core/PathSensitive/CallEvent.h"
#include "clang/StaticAnalyzer/Core/PathSensitive/CheckerContext.h"

using namespace clang;
using namespace ento;

namespace {
class APIMisuse : public BugType {
public:
	APIMisuse(const CheckerBase *checker, const char *name)
			: BugType(checker, name, "Nuclear Reactor API Error") {}
};
}

class ReactorState {
private:
	enum Kind { On, Off } K;
public:
	ReactorState(unsigned InK) : K((Kind)InK) {}
	bool isOn() const { return K == On; }
	bool isOff() const { return K == Off; }
	static unsigned getOn() { return (unsigned)On; }
	static unsigned getOff() { return (unsigned)Off; }
	bool operator==(const ReactorState &X) const {
		return K == X.K;
	}
	void Profile(llvm::FoldingSetNodeID &ID) const {
		ID.AddInteger(K);
	}
};

class ReactorChecker : public Checker<check::PostCall> {
	mutable IdentifierInfo *IIturnReactorOn, *IISCRAM;
	std::unique_ptr<BugType> DoubleSCRAMBugType;
	std::unique_ptr<BugType> DoubleOnBugType;
	void initIdentifierInfo(ASTContext &Ctx) const;
	void reportDoubleSCRAM(const CallEvent &call, CheckerContext &C) const;
	void reportDoubleOn(const CallEvent &call, CheckerContext &C) const;
public:
	ReactorChecker();
	void checkPostCall(const CallEvent &call, CheckerContext &C) const;
};

REGISTER_MAP_WITH_PROGRAMSTATE(RS, int, ReactorState)

ReactorChecker::ReactorChecker() : IIturnReactorOn(0), IISCRAM(0) {
	DoubleSCRAMBugType.reset(new APIMisuse(this, "Double SCRAM"));
	DoubleOnBugType.reset(new APIMisuse(this, "Double On"));
}

void ReactorChecker::initIdentifierInfo(ASTContext &Ctx) const {
	if (IIturnReactorOn)
		return;
	IIturnReactorOn = &Ctx.Idents.get("turnReactorOn");
	IISCRAM = &Ctx.Idents.get("SCRAM");
}

void ReactorChecker::checkPostCall(const CallEvent &call, CheckerContext &C) const {
	initIdentifierInfo(C.getASTContext());
	if (!call.isGlobalCFunction())
		return;
	if (call.getCalleeIdentifier() == IIturnReactorOn) {
		ProgramStateRef State = C.getState();
		const ReactorState *S = State->get<RS>(1);
		if (S && S->isOn()) {
			reportDoubleOn(call, C);
			return;
		}
		State = State->set<RS>(1, ReactorState::getOn());
		C.addTransition(State);
		return;
	}
	if (call.getCalleeIdentifier() == IISCRAM) {
		ProgramStateRef State = C.getState();
		const ReactorState *S = State->get<RS>(1);
		if (S && S->isOn()) {
			reportDoubleSCRAM(call, C);
			return;
		}
		State = State->set<RS>(1, ReactorState::getOn());
		C.addTransition(State);
		return;
	}
}

void ReactorChecker::reportDoubleOn(const CallEvent &call, CheckerContext &C) const {
	ExplodedNode *ErrNode = C.generateSink(C.getState(), C.getPredecessor());
	if (!ErrNode)
		return;
	auto R = std::make_unique<PathSensitiveBugReport>(*DoubleOnBugType, "Turned on the reactor two times", ErrNode);
	R->addRange(call.getSourceRange());
	C.emitReport(std::move(R));
}

void ReactorChecker::reportDoubleSCRAM(const CallEvent &call, CheckerContext &C) const {
	ExplodedNode *ErrNode = C.generateSink(C.getState(), C.getPredecessor());
	if (!ErrNode)
		return;
	auto R = std::make_unique<PathSensitiveBugReport>(*DoubleSCRAMBugType, "Called a SCRAM procedure twice", ErrNode);
	R->addRange(call.getSourceRange());
	C.emitReport(std::move(R));
}

namespace clang {
namespace ento {
	void registerReactorChecker(CheckerManager &mgr) {
		mgr.registerChecker<ReactorChecker>();
	}

	bool shouldRegisterReactorChecker(const CheckerManager &mgr) { 
		return true;
   	}
}
}

