import XCTest

#if canImport(PureParserC)
import PureParserC
#endif

final class PureParserExamples: XCTestCase {
    static var allTests = [
        ("test_Plain", test_Plain),
        ("test_Variable", test_Variable),
        ("test_VariableAndBlockWithVariable", test_VariableAndBlockWithVariable),
        ("test_VariableAndBlockWithPlain", test_VariableAndBlockWithPlain),
        ("test_BlockWithRich", test_BlockWithRich),
        ("test_InactiveBlock", test_InactiveBlock),
        ("test_Complex", test_Complex),
    ]
    
    func test_Plain() {
        let parser = PureParser()
        let formula = "Hello world"
        
        XCTAssertEqual(
            parser.execute(formula, collapseSpaces: true, resetOnFinish: true),
            "Hello world"
        )
    }
    
    func test_Variable() {
        let parser = PureParser()
        let formula = "Hello, $name"
        
        parser.assign(variable: "name", value: "Stan")
        
        XCTAssertEqual(
            parser.execute(formula, collapseSpaces: true, resetOnFinish: true),
            "Hello, Stan"
        )
    }
    
    func test_VariableAndBlockWithVariable() {
        let parser = PureParser()
        let formula = "Please wait, $name: we're calling $[$anotherName ## another guy]"
        
        parser.assign(variable: "name", value: "Stan")
        parser.assign(variable: "anotherName", value: "Paul")

        XCTAssertEqual(
            parser.execute(formula, collapseSpaces: true, resetOnFinish: true),
            "Please wait, Stan: we're calling Paul"
        )
    }
    
    func test_VariableAndBlockWithPlain() {
        let parser = PureParser()
        let formula = "Please wait, $name: we're calling $[$anotherName ## another guy]"
        
        parser.assign(variable: "name", value: "Stan")

        XCTAssertEqual(
            parser.execute(formula, collapseSpaces: true, resetOnFinish: true),
            "Please wait, Stan: we're calling another guy"
        )
    }
    
    func test_BlockWithRich() {
        let parser = PureParser()
        let formula = "Congrats! You saved it $[in folder '$folder']."
        
        parser.assign(variable: "folder", value: "Documents")

        XCTAssertEqual(
            parser.execute(formula, collapseSpaces: true, resetOnFinish: true),
            "Congrats! You saved it in folder 'Documents'."
        )
    }
    
    func test_InactiveBlock() {
        let parser = PureParser()
        let formula = "Congrats! You saved it $[in folder '$folder']."
        
        XCTAssertEqual(
            parser.execute(formula, collapseSpaces: true, resetOnFinish: true),
            "Congrats! You saved it."
        )
    }
    
    func test_Complex() {
        let parser = PureParser()
        let formula = "$[Agent $creatorName ## You] changed reminder $[«$comment»] $[:target: for $[$targetName ## you]] on $date at $time"
        
        parser.assign(variable: "comment", value: "Check his payment")
        parser.assign(variable: "date", value: "today")
        parser.assign(variable: "time", value: "11:30 AM")
        parser.activate(alias: "target", true)

        XCTAssertEqual(
            parser.execute(formula, collapseSpaces: true, resetOnFinish: true),
            "You changed reminder «Check his payment» for you on today at 11:30 AM"
        )
    }
}
