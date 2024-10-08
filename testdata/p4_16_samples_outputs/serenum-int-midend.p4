#include <core.p4>

header Ethernet {
    bit<48> src;
    bit<48> dest;
    int<24> type;
}

struct Headers {
    Ethernet eth;
}

parser prs(packet_in p, out Headers h) {
    @name("prs.e") Ethernet e_0;
    state start {
        e_0.setInvalid();
        p.extract<Ethernet>(e_0);
        transition select(e_0.type) {
            24s0x800: accept;
            24s0x806: accept;
            default: reject;
        }
    }
}

control c(inout Headers h) {
    @hidden action serenumint41() {
        h.eth.setInvalid();
    }
    @hidden action serenumint43() {
        h.eth.type = 24s0;
    }
    @hidden table tbl_serenumint41 {
        actions = {
            serenumint41();
        }
        const default_action = serenumint41();
    }
    @hidden table tbl_serenumint43 {
        actions = {
            serenumint43();
        }
        const default_action = serenumint43();
    }
    apply {
        if (h.eth.isValid()) {
            if (h.eth.type == 24s0x800) {
                tbl_serenumint41.apply();
            } else {
                tbl_serenumint43.apply();
            }
        } else {
            ;
        }
    }
}

parser p<H>(packet_in _p, out H h);
control ctr<H>(inout H h);
package top<H>(p<H> _p, ctr<H> _c);
top<Headers>(prs(), c()) main;
