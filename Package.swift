// swift-tools-version:5.1

import PackageDescription

let package = Package(
    name: "PureParser",
    products: [
        .library(
            name: "PureParser",
            targets: ["PureParserCpp", "PureParserC", "PureParser"])
    ],
    dependencies: [
    ],
    targets: [
        .target(
            name: "PureParserCpp",
            dependencies: [],
            path: "cpp_src",
            sources: [
                "PureScanner.cpp", "PureParser.cpp"
            ],
            publicHeadersPath: "."),
        .target(
            name: "PureParserC",
            dependencies: ["PureParserCpp"],
            path: "c_wrapper",
            sources: [
                "pure_parser.cpp"
            ],
            publicHeadersPath: "."),
        .target(
            name: "PureParser",
            dependencies: ["PureParserC"],
            path: "swift_wrapper",
            sources: [
                "PureParser.swift"
            ])
    ],
    cxxLanguageStandard: .cxx1z
)
