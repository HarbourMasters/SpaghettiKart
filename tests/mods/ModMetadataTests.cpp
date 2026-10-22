#include "ModMetadata.h"

#include <iostream>
#include <string>

namespace {
int checks = 0;

void Require(bool condition, const std::string& message) {
    ++checks;
    if (!condition) {
        throw std::runtime_error(message);
    }
}

std::string Mod(const std::string& version) {
    return "[mod]\nname = 'test'\nversion = '" + version + "'\n";
}

void ExpectError(const std::string& input, const std::string& field, const std::string& value) {
    try {
        const auto metadata = ModMetadata::LoadFromTOML(input);
        (void) metadata;
    } catch (const std::runtime_error& error) {
        const std::string message = error.what();
        Require(message.find(field) != std::string::npos, "Missing field in diagnostic: " + message);
        Require(message.find(value) != std::string::npos, "Missing value in diagnostic: " + message);
        return;
    }
    throw std::runtime_error("Invalid metadata was accepted: " + input);
}

void CheckRange(const std::string& range, const std::string& version, bool expected) {
    const auto metadata = ModMetadata::LoadFromTOML(Mod("1.2.3") + "[dependencies]\nother = '" + range + "'\n");
    const auto candidate = semver::from_string(version);
    const auto& dependency = metadata.dependencies.at("other");
    Require(dependency.second == range, "Original dependency range was not preserved");
    Require(dependency.first.contains(candidate) == expected, range + " membership for " + version);
}
} // namespace

int main() {
    try {
        Require(SEMVER_VERSION_MAJOR == 1 && SEMVER_VERSION_MINOR == 0 && SEMVER_VERSION_PATCH == 1,
                "Tests must use the project's semver v1.0.1 pin");
        Require(TOML_LIB_MAJOR == 3 && TOML_LIB_MINOR == 4 && TOML_LIB_PATCH == 0,
                "Tests must use the project's toml++ v3.4.0 pin");

        for (const std::string version :
             { "garbage", "1.2.3garbage", "", "1.2", "01.2.3", "-1.2.3", "4294967296.0.0" }) {
            // No own or reverse dependencies: failure must happen in the parser itself.
            ExpectError(Mod(version), "mod.version", "'" + version + "'");
        }
        for (const std::string range : { "garbage", ">=1.0.0 garbage", ">=1.0.0 <garbage", ">=1.0.0 || garbage", "",
                                         " ", "^garbage", "1.2.3+build" }) {
            ExpectError(Mod("1.2.3") + "[dependencies]\nother = '" + range + "'\n", "dependency 'other'",
                        "'" + range + "'");
        }
        ExpectError(Mod("1.2.3") + "[dependencies]\ngood = '>=1.0.0'\nother = 'garbage'\n", "dependency 'other'",
                    "garbage");
        ExpectError(Mod("1.2.3") + "broken = [", "Invalid TOML", "");

        for (const std::string version :
             { "1.2.3", "1.0.0-alpha1", "1.2.3+build", "1.2.3-alpha.1+build.2", "4294967295.0.0" }) {
            const auto metadata = ModMetadata::LoadFromTOML(Mod(version));
            Require(metadata.name == "test", "Mod name changed");
            Require(metadata.version.to_string() == version, "Version did not round-trip: " + version);
            Require(metadata.dependencies.empty(), "Unexpected dependencies");
        }

        CheckRange(">=1.0.0 <2.0.0", "1.5.0", true);
        CheckRange(">=1.0.0 <2.0.0", "2.0.0", false);
        CheckRange(">=1.0.0 <2.0.0 || >=3.0.0", "3.0.0", true);
        CheckRange(">=1.0.0 <2.0.0 || >=3.0.0", "2.5.0", false);
        CheckRange(">=1.0.0 ", "2.0.0", true);
        CheckRange("1.0.0-alpha1", "1.0.0-alpha1", true);
        CheckRange("=1.0.0-alpha1", "1.0.0-alpha1", true);
        CheckRange("1.0.0-alpha1", "1.0.0", false);
        CheckRange(">=1.0.0-alpha1", "1.0.0-alpha2", true);
        CheckRange(">=1.0.0-alpha1", "1.1.0-alpha1", false);
        CheckRange(">=1.0.0", "1.1.0-alpha1", false);
        CheckRange("1.2.3", "1.2.3+build", true);

        // Stable semver adds caret, tilde, partial and wildcard ranges.
        CheckRange("^1.0.0", "1.5.0", true);
        CheckRange("^1.0.0", "2.0.0", false);
        CheckRange("~1.2.3", "1.2.4", true);
        CheckRange("~1.2.3", "1.3.0", false);
        CheckRange("1.2", "1.2.9", true);
        CheckRange("1.2.*", "1.3.0", false);
        CheckRange("*", "42.0.0", true);
        CheckRange("*", "42.0.0-alpha1", false);

        // Missing fields retain the existing defaults; this patch validates strings.
        const auto defaults = ModMetadata::LoadFromTOML("");
        Require(defaults.name.empty() && defaults.version.to_string() == "0.1.0" && defaults.dependencies.empty(),
                "Missing-field policy changed");

        std::cout << checks << " metadata checks passed\n";
        return 0;
    } catch (const std::exception& error) {
        std::cerr << "FAIL: " << error.what() << '\n';
        return 1;
    }
}
