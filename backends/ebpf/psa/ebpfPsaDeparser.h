/*
Copyright 2022-present Orange
Copyright 2022-present Open Networking Foundation

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/
#ifndef BACKENDS_EBPF_PSA_EBPFPSADEPARSER_H_
#define BACKENDS_EBPF_PSA_EBPFPSADEPARSER_H_

#include "backends/ebpf/ebpfDeparser.h"
#include "backends/ebpf/psa/ebpfPsaParser.h"
#include "backends/ebpf/psa/externs/ebpfPsaChecksum.h"
#include "backends/ebpf/psa/externs/ebpfPsaDigest.h"
#include "ebpfPsaControl.h"
#include "lib/rtti.h"

namespace P4::EBPF {

class EBPFDeparserPSA;

class DeparserBodyTranslatorPSA : public DeparserBodyTranslator {
 public:
    explicit DeparserBodyTranslatorPSA(const EBPFDeparserPSA *deparser);

    void processFunction(const P4::ExternFunction *function) override;
    void processMethod(const P4::ExternMethod *method) override;
};

class EBPFDeparserPSA : public EBPFDeparser {
 public:
    const IR::Parameter *user_metadata;
    const IR::Parameter *istd;
    const IR::Parameter *resubmit_meta;
    std::map<cstring, EBPFChecksumPSA *> checksums;
    std::map<cstring, EBPFDigestPSA *> digests;

    EBPFDeparserPSA(const EBPFProgram *program, const IR::ControlBlock *control,
                    const IR::Parameter *parserHeaders, const IR::Parameter *istd)
        : EBPFDeparser(program, control, parserHeaders), istd(istd) {
        codeGen = new DeparserBodyTranslatorPSA(this);
    }

    void emitTypes(CodeBuilder *builder) const;
    void emitDigestInstances(CodeBuilder *builder) const;
    void emitDeclaration(CodeBuilder *builder, const IR::Declaration *decl) override;

    EBPFChecksumPSA *getChecksum(cstring name) const {
        auto result = ::P4::get(checksums, name);
        BUG_CHECK(result != nullptr, "No checksum named %1%", name);
        return result;
    }

    EBPFDigestPSA *getDigest(cstring name) const {
        auto result = ::P4::get(digests, name);
        BUG_CHECK(result != nullptr, "No digest named %1%", name);
        return result;
    }

    void emitDeparserExternCalls(EBPF::CodeBuilder *builder) override {
        controlBlock->container->body->apply(*codeGen);
        builder->newline();
    }

    DECLARE_TYPEINFO(EBPFDeparserPSA, EBPFDeparser);
};

class IngressDeparserPSA : public EBPFDeparserPSA {
 public:
    IngressDeparserPSA(const EBPFProgram *program, const IR::ControlBlock *control,
                       const IR::Parameter *parserHeaders, const IR::Parameter *istd)
        : EBPFDeparserPSA(program, control, parserHeaders, istd) {}

    bool build() override;

    DECLARE_TYPEINFO(IngressDeparserPSA, EBPFDeparserPSA);
};

class EgressDeparserPSA : public EBPFDeparserPSA {
 public:
    EgressDeparserPSA(const EBPFProgram *program, const IR::ControlBlock *control,
                      const IR::Parameter *parserHeaders, const IR::Parameter *istd)
        : EBPFDeparserPSA(program, control, parserHeaders, istd) {}

    bool build() override;

    DECLARE_TYPEINFO(EgressDeparserPSA, EBPFDeparserPSA);
};

class TCIngressDeparserPSA : public IngressDeparserPSA {
 public:
    TCIngressDeparserPSA(const EBPFProgram *program, const IR::ControlBlock *control,
                         const IR::Parameter *parserHeaders, const IR::Parameter *istd)
        : IngressDeparserPSA(program, control, parserHeaders, istd) {}

    void emitPreDeparser(CodeBuilder *builder) override;

    DECLARE_TYPEINFO(TCIngressDeparserPSA, IngressDeparserPSA);
};

class TCEgressDeparserPSA : public EgressDeparserPSA {
 public:
    TCEgressDeparserPSA(const EBPFProgram *program, const IR::ControlBlock *control,
                        const IR::Parameter *parserHeaders, const IR::Parameter *istd)
        : EgressDeparserPSA(program, control, parserHeaders, istd) {}

    DECLARE_TYPEINFO(TCEgressDeparserPSA, EgressDeparserPSA);
};

class TCIngressDeparserForTrafficManagerPSA : public TCIngressDeparserPSA {
 public:
    TCIngressDeparserForTrafficManagerPSA(const EBPFProgram *program,
                                          const IR::ControlBlock *control,
                                          const IR::Parameter *parserHeaders,
                                          const IR::Parameter *istd)
        : TCIngressDeparserPSA(program, control, parserHeaders, istd) {}
    void emitPreDeparser(CodeBuilder *builder) override;
    void emitDeparserExternCalls(CodeBuilder *builder) override {
        (void)builder;
        // do not emit deparser extern calls for TCIngressDeparserForTrafficManagerPSA
    }

    DECLARE_TYPEINFO(TCIngressDeparserForTrafficManagerPSA, TCIngressDeparserPSA);
};

class XDPIngressDeparserPSA : public IngressDeparserPSA {
 public:
    XDPIngressDeparserPSA(const EBPFProgram *program, const IR::ControlBlock *control,
                          const IR::Parameter *parserHeaders, const IR::Parameter *istd)
        : IngressDeparserPSA(program, control, parserHeaders, istd) {}

    void emitPreDeparser(CodeBuilder *builder) override;

    DECLARE_TYPEINFO(XDPIngressDeparserPSA, IngressDeparserPSA);
};

class XDPEgressDeparserPSA : public EgressDeparserPSA {
 public:
    XDPEgressDeparserPSA(const EBPFProgram *program, const IR::ControlBlock *control,
                         const IR::Parameter *parserHeaders, const IR::Parameter *istd)
        : EgressDeparserPSA(program, control, parserHeaders, istd) {}

    void emitPreDeparser(CodeBuilder *builder) override;

    DECLARE_TYPEINFO(XDPEgressDeparserPSA, EgressDeparserPSA);
};

}  // namespace P4::EBPF

#endif /* BACKENDS_EBPF_PSA_EBPFPSADEPARSER_H_ */
