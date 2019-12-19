fileprivate struct Output {
    let formula: String
    let result: String
    let reference: String
    
    var hasPassed: Bool {
        return (result == reference)
    }
}

fileprivate func execute_Plain() -> Output {
    let parser = PureParser()
    let formula = "Hello world"
    
    return Output(
        formula: formula,
        result: parser.execute(formula, collapseSpaces: true, resetOnFinish: true),
        reference: "Hello world"
    )
}

fileprivate func execute_Variable() -> Output {
    let parser = PureParser()
    let formula = "Hello, $name"
    
    parser.assign(variable: "name", value: "Stan")
    
    return Output(
        formula: formula,
        result: parser.execute(formula, collapseSpaces: true, resetOnFinish: true),
        reference: "Hello, Stan"
    )
}

fileprivate func execute_VariableAndBlockWithVariable() -> Output {
    let parser = PureParser()
    let formula = "Please wait, $name: we're calling $[$anotherName ## another guy]"
    
    parser.assign(variable: "name", value: "Stan")
    parser.assign(variable: "anotherName", value: "Paul")

    return Output(
        formula: formula,
        result: parser.execute(formula, collapseSpaces: true, resetOnFinish: true),
        reference: "Please wait, Stan: we're calling Paul"
    )
}

fileprivate func execute_VariableAndBlockWithPlain() -> Output {
    let parser = PureParser()
    let formula = "Please wait, $name: we're calling $[$anotherName ## another guy]"
    
    parser.assign(variable: "name", value: "Stan")

    return Output(
        formula: formula,
        result: parser.execute(formula, collapseSpaces: true, resetOnFinish: true),
        reference: "Please wait, Stan: we're calling another guy"
    )
}

fileprivate func execute_BlockWithRich() -> Output {
    let parser = PureParser()
    let formula = "Congrats! You saved it $[in folder '$folder']."
    
    parser.assign(variable: "folder", value: "Documents")

    return Output(
        formula: formula,
        result: parser.execute(formula, collapseSpaces: true, resetOnFinish: true),
        reference: "Congrats! You saved it in folder 'Documents'."
    )
}

fileprivate func execute_InactiveBlock() -> Output {
    let parser = PureParser()
    let formula = "Congrats! You saved it $[in folder '$folder']."
    
    return Output(
        formula: formula,
        result: parser.execute(formula, collapseSpaces: true, resetOnFinish: true),
        reference: "Congrats! You saved it."
    )
}

fileprivate func execute_Complex() -> Output {
    let parser = PureParser()
    let formula = "$[Agent $creatorName ## You] changed reminder $[«$comment»] $[:target: for $[$targetName ## you]] on $date at $time"
    
    parser.assign(variable: "comment", value: "Check his payment")
    parser.assign(variable: "date", value: "today")
    parser.assign(variable: "time", value: "11:30 AM")
    parser.activate(alias: "target", true)

    return Output(
        formula: formula,
        result: parser.execute(formula, collapseSpaces: true, resetOnFinish: true),
        reference: "You changed reminder «Check his payment» for you on today at 11:30 AM"
    )
}

#if OUTER_EXECUTION
import PureParser

final class PureParserExamples {
    func run() {
        let cases = [
            ("test_Plain", execute_Plain),
            ("test_Variable", execute_Variable),
            ("test_VariableAndBlockWithVariable", execute_VariableAndBlockWithVariable),
            ("test_VariableAndBlockWithPlain", execute_VariableAndBlockWithPlain),
            ("test_BlockWithRich", execute_BlockWithRich),
            ("test_InactiveBlock", execute_InactiveBlock),
            ("test_Complex", execute_Complex)
        ]
        
        for (name, test) in cases {
            let out = test()
            print("Testing '\(name)'")
            print("> Formula: \(out.formula)")

            if out.hasPassed {
                print("[OK]")
            }
            else {
                print("> Output: \(out.result)")
                print("> Reference: \(out.reference)")
                print("[Failed]")
                assertionFailure()
            }

            print(" ")
        }
        
        print("== All test cases passed OK ==")
    }
}
#else
import XCTest

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
        let output = execute_Plain()
        XCTAssertEqual(output.result, output.reference)
    }

    func test_Variable() {
        let output = execute_Variable()
        XCTAssertEqual(output.result, output.reference)
    }

    func test_VariableAndBlockWithVariable() {
        let output = execute_VariableAndBlockWithVariable()
        XCTAssertEqual(output.result, output.reference)
    }

    func test_VariableAndBlockWithPlain() {
        let output = execute_VariableAndBlockWithPlain()
        XCTAssertEqual(output.result, output.reference)
    }

    func test_BlockWithRich() {
        let output = execute_BlockWithRich()
        XCTAssertEqual(output.result, output.reference)
    }

    func test_InactiveBlock() {
        let output = execute_InactiveBlock()
        XCTAssertEqual(output.result, output.reference)
    }

    func test_Complex() {
        let output = execute_Complex()
        XCTAssertEqual(output.result, output.reference)
    }
}
#endif
