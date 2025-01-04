/**
 * Copyright (C) 2024 Intel Corporation
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may not
 * use this file except in compliance with the License.  You may obtain a copy
 * of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software distributed
 * under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
 * CONDITIONS OF ANY KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations under the License.
 *
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef BF_P4C_LOGGING_SOURCE_INFO_LOGGING_H_
#define BF_P4C_LOGGING_SOURCE_INFO_LOGGING_H_

// #define BF_P4C_LOGGING_SOURCE_INFO_LOGGING_H_DEBUG

#include "backends/tofino/bf-p4c/logging/source_info_schema.h"
#include "frontends/common/resolveReferences/referenceMap.h"
#include "ir/ir.h"

// Logging::Source_Info_Schema_Logger is generated by generate_logging.py
using Source_Info_Schema_Logger = Logging::Source_Info_Schema_Logger;

/**
 * \ingroup midend
 * \brief Pass that collects source information.
 *
 * This inspector serves to collect source information about definitions and uses
 * of all P4 symbols in the program. It shall be applied when all important
 * IR transformations have been done and P4::ReferenceMap is still available.
 */
class CollectSourceInfoLogging : public Inspector {
 public:
    // Source info structure for a P4 symbol
    struct Symbol {
        cstring name;
        cstring type;
        Util::SourceInfo inst;
        std::set<Util::SourceInfo> refs;

        Symbol(cstring name, cstring type, Util::SourceInfo inst)
            : name(name), type(type), inst(inst) {}
        Symbol() = default;
    };

    // Constructor (requires P4::ReferenceMap)
    explicit CollectSourceInfoLogging(const P4::ReferenceMap &);

    // This method can be used to add symbols later after the pass is applied
    void addSymbol(const Symbol &);

 private:
    // Reference map is used to map uses of P4 symbols to their definitions
    const P4::ReferenceMap &refMap;
    // Map from P4 symbol names to their source info (used during the pass)
    std::map<cstring, std::map<cstring, Symbol>> symbolByTypeMap;

    // P4 program source root (base for all relative file paths)
    std::string sourceRoot;
    // List of P4 symbols' source info (used for export)
    std::list<Source_Info_Schema_Logger::SymbolDefinition *> symbols;

    // Init sets "sourceRoot"
    profile_t init_apply(const IR::Node *root) override;
    // The pass over IR::Paths should be enough to gather all P4 symbols definitions and uses
    bool preorder(const IR::Path *) override;
    // ! The other "preorder()"s are needed because of a bug in the inlining of nested blocks that
    // ! causes source info to point to the nested block apply() instead of the actual symbol use
    bool preorder(const IR::Argument *) override;
    bool preorder(const IR::Member *) override;
    // TODO: Parser states are not covered
    // End populates "symbols" by calling "addSymbol()"
    void end_apply() override;

    // Adds a symbol reference source info into "symbolByTypeMap" (called by "preorder()")
    void addSymbolReference(const IR::Node *, const IR::IDeclaration *);

    friend class SourceInfoLogging;
};

/*
 * This inspector serves to log source information collected by CollectSourceInfoLogging.
 */
class SourceInfoLogging : public Inspector {
 private:
    const CollectSourceInfoLogging &sourceInfo;
    Source_Info_Schema_Logger logger;
    std::string manifestPath;  // path to source.json relative to manifest.json

#ifdef BF_P4C_LOGGING_SOURCE_INFO_LOGGING_H_DEBUG
    std::map<std::string, std::map<unsigned, std::set<std::string>>> sourceCoverage;
#endif

 public:
    explicit SourceInfoLogging(const std::string &outdir, const std::string &filename,
                               const CollectSourceInfoLogging &sourceInfo)
        : Inspector(),
          sourceInfo(sourceInfo),
          logger((outdir + "/" + filename).c_str(), SOURCE_INFO_SCHEMA_VERSION,
                 sourceInfo.sourceRoot) {
        manifestPath = filename;

        for (const auto &symbol : sourceInfo.symbols) {
            logger.append(symbol);

#ifdef BF_P4C_LOGGING_SOURCE_INFO_LOGGING_H_DEBUG
            sourceCoverage[symbol->get_declaration()->get_file()]
                          [symbol->get_declaration()->get_line()]
                              .insert(std::string(symbol->get_symbol_name()) + "(" +
                                      symbol->get_symbol_type() + ")");
            for (auto *ref : symbol->get_references()) {
                sourceCoverage[ref->get_file()][ref->get_line()].insert(
                    std::string(symbol->get_symbol_name()) + "(" + symbol->get_symbol_type() + ")");
            }
#endif
        }
#ifdef BF_P4C_LOGGING_SOURCE_INFO_LOGGING_H_DEBUG
        for (auto file : sourceCoverage) {
            for (auto line : file.second) {
                std::cout << "COVERAGE ";
                if (!file.first.empty() && file.first.front() != '/')
                    std::cout << sourceInfo.sourceRoot << "/";
                std::cout << file.first << ":" << line.first;
                for (auto ref : line.second) {
                    std::cout << " " << ref;
                }
                std::cout << std::endl;
            }
        }
#endif
    }

    bool preorder(const IR::Node *) override { return false; }
    void end_apply() override;
};

#endif /* BF_P4C_LOGGING_SOURCE_INFO_LOGGING_H_ */