
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/AST/Attr.h"
#include "clang/Tooling/RefactoringCallbacks.h"
#include "RewriterTestContext.h"

using namespace llvm;
using namespace clang;
using namespace clang::tooling;
using namespace clang::ast_matchers;

template<typename T>
void ReplaceMatcherResultByCallback(const std::string &code, const T &matcher, RefactoringCallback &callback) {
	std::map<std::string, Replacements> FileToReplace;
	ASTMatchRefactorer Finder(FileToReplace);
	Finder.addMatcher(traverse(TK_AsIs, matcher), &callback);
	std::unique_ptr<FrontendActionFactory> Factory(
					newFrontendActionFactory(&Finder));
	if (!runToolOnCode(Factory->create(), code)) {
		errs() << "Parsing error in \"" << code << "\"\n";
		return;
	}
	RewriterTestContext context;
	FileID id = context.createInMemoryFile("input.cc", code);
	if (!applyAllReplacements(FileToReplace["input.cc"], context.Rewrite)) {
		errs() << "Repalce erros in \"" << code << "\"\n";
		return;
	}
	outs() << "\"" << code << 
			"\"\n Replace to \n" << 
			"\"" << context.getRewrittenText(id) << "\"\n"
			<< "----------------------------------\n";
	return;
}

class ReplaceStmtFuncNameWithText : public RefactoringCallback {
public:
	ReplaceStmtFuncNameWithText(StringRef FromID, StringRef ToText) :
		FromID(std::string(FromID)), ToText(std::string(ToText)), 
	   	SectionName("test_section") {}

	void run(const MatchFinder::MatchResult &Result) override {
		if (const FunctionDecl *FromMatch = 
				Result.Nodes.getNodeAs<FunctionDecl>(FromID)) {
			auto NameInfo = FromMatch->getNameInfo();
			auto Err = Replace.add(Replacement(
				*Result.SourceManager, 
				CharSourceRange::getTokenRange(NameInfo.getSourceRange()), 
				ToText));
			if (Err) {
				errs() << toString(std::move(Err)) << "\n";
			}
			if (FromMatch->hasAttrs()) {
				auto &vec = FromMatch->getAttrs();
			   	for (auto * attr : vec) {
					errs() << attr->getSpelling() << "\n";
				}
			} else {
				errs() << "FromMatch has NO Attr\n";
			}
		} else {
			errs() << "Not Match FunctionDecl\n";
		}
	}

private:
	std::string FromID;
	std::string ToText;
	std::string SectionName;
	//std::unique_ptr<SectionAttr> attr;
};

class ReplaceAddFuncNameWithSection : public RefactoringCallback {
public:
	ReplaceAddFuncNameWithSection(StringRef FromID, 
					StringRef SectionName) :
		FromID(std::string(FromID)), 
		SectionName(std::string("__section__((\"")
						.append(std::string(SectionName))
						.append("\"))")) {}

	void run(const MatchFinder::MatchResult &Result) override {
		if (const FunctionDecl *FromMatch = 
				Result.Nodes.getNodeAs<FunctionDecl>(FromID)) {
			auto NameInfo = FromMatch->getNameInfo();
			auto Err = Replace.add(Replacement(
				*Result.SourceManager, 
				CharSourceRange::getTokenRange(NameInfo.getSourceRange()), 
				SectionName
					.append(" ")
					.append(NameInfo.getName().getAsString())));
			if (Err) {
				errs() << toString(std::move(Err)) << "\n";
			}
		} else {
			errs() << "Not Match FunctionDecl\n";
		}
	}

private:
	std::string FromID;
	std::string SectionName;
};

#include <tuple>
#include <utility>


const char * FuncNames[] = {
#include "func_name.txt"
};


template<typename Array, std::size_t... I>
constexpr auto arrToTuple(const Array& a, std::index_sequence<I...>) {
	return std::make_tuple(a[I]...);
}

template<typename Tuple, std::size_t... I>
auto genAnyOfMatcher_helper(const Tuple& t, std::index_sequence<I...>) {
	return anyOf(hasName(std::get<I>(t))...);
}

template<typename Array, std::size_t... I>
auto genAnyOfMatcher_impl(const Array& a, std::index_sequence<I...>) {
	auto t = std::make_tuple(a[I]...);
	return anyOf(hasName(std::get<I>(t))...);
}

template <typename T, std::size_t N>
auto genAnyOfMatcher(T const (&p)[N]) {
	auto Is = std::make_index_sequence<N>{};
	//return genAnyOfMatcher_helper(arrToTuple(p, Is), Is);
	return genAnyOfMatcher_impl(p, Is);
}

int main() {
	{
		std::string Code = "void f() { int i = 1;}";
		ReplaceStmtWithText Callback("id", ";");
		ReplaceMatcherResultByCallback(Code, declStmt().bind("id"), 
						Callback);
	}
	{
		std::string Code = "void f() { int i = 1;}";
		ReplaceStmtFuncNameWithText Callback("id", "g");
		ReplaceMatcherResultByCallback(Code, 
						functionDecl(hasName("f")).bind("id"), Callback);
	}
	{
		std::string Code = "void f() { int i = 1;}";
		ReplaceAddFuncNameWithSection Callback("id", "test_section");
		//auto m = anyOf(hasName("foo"), hasName("bar"));
		auto m = genAnyOfMatcher(FuncNames);
		ReplaceMatcherResultByCallback(Code, 
						functionDecl(m).bind("id"), Callback);
	}
	{
		std::string Code = "void foo() { int i = 1;}";
		ReplaceAddFuncNameWithSection Callback("id", "test_section");
		auto m = anyOf(hasName("foo"), hasName("bar"));
		ReplaceMatcherResultByCallback(Code, 
						functionDecl(m).bind("id"), Callback);
	}
	{
		std::string Code = "#define A void f() { int i = 1;}\nA";
		ReplaceStmtWithText Callback("id", ";");
		ReplaceMatcherResultByCallback(Code, declStmt().bind("id"), 
						Callback);
	}
	{
		std::string Code = "void f() { int i = 1;}";
		ReplaceStmtWithText Callback("id", "2");
		ReplaceMatcherResultByCallback(Code, 
						expr(integerLiteral()).bind("id"), Callback);
	}
	{
		std::string Code = 
				"bool a; void f() { if(a) f(); else a = true; }";
		ReplaceIfStmtWithItsBody Callback("id", true);
		ReplaceMatcherResultByCallback(Code, 
						ifStmt(hasCondition(
								implicitCastExpr(
									hasSourceExpression(
										declRefExpr(
											to(varDecl(hasName("a")))))))
								).bind("id"), 
						Callback);
	}

	return 0;
}

