var srs_qp =
[
    [ "Purpose and Scope", "srs-qp.html#srs-qp_scope", [
      [ "About the QP Framework Family", "srs-qp.html#srs-qp_about", null ],
      [ "Audience", "srs-qp.html#srs-qp_audience", null ],
      [ "Document Organization", "srs-qp.html#srs-qp_org", null ],
      [ "Document Conventions", "srs-qp.html#srs-qp_conv", [
        [ "General Requirement UIDs", "srs-qp.html#srs-qp_uid", null ],
        [ "Use of \"shall\"", "srs-qp.html#srs-qp_conv-shall", null ],
        [ "Use of \"shall not\"", "srs-qp.html#srs-qp_conv-shall-not", null ],
        [ "Use of \"should\"", "srs-qp.html#srs-qp_conv-should", null ],
        [ "Use of \"may\"", "srs-qp.html#srs-qp_conv-may", null ]
      ] ],
      [ "References", "srs-qp.html#srs-qp_ref", null ]
    ] ],
    [ "Overview", "srs-qp_over.html", [
      [ "Goals", "srs-qp_over.html#srs-qp_over-goals", null ],
      [ "Context of Use", "srs-qp_over.html#srs-qp_over-ctxt", [
        [ "Inversion of Control", "srs-qp_over.html#srs-qp_over_inv", null ],
        [ "Framework, NOT a Library", "srs-qp_over.html#srs-qp_over_frame", null ],
        [ "Deriving Application from a Framework", "srs-qp_over.html#srs-qp_over-derive", null ]
      ] ],
      [ "Main Use Case", "srs-qp_over.html#srs-qp_over-use", null ],
      [ "Portability & Configurability", "srs-qp_over.html#srs-qp_over-conf", [
        [ "Compile-Time Configurability", "srs-qp_over.html#srs-qp_conf_compile", null ],
        [ "Run-Time Configurability", "srs-qp_over.html#srs-qp_conf_run", null ]
      ] ]
    ] ],
    [ "Active Objects", "srs-qp_ao.html", [
      [ "Concepts & Definitions", "srs-qp_ao.html#srs-qp_ao-intro", [
        [ "Active Object Model of Computation", "srs-qp_ao.html#srs-qp_ao-model", null ],
        [ "Active Objects", "srs-qp_ao.html#srs-qp_ao-def", null ],
        [ "Encapsulation for Concurrency", "srs-qp_ao.html#srs-qp_ao-enc", null ],
        [ "Shared-Nothing Principle", "srs-qp_ao.html#srs-qp_ao-sharing", null ],
        [ "Execution Context", "srs-qp_ao.html#srs-qp_ao-exec", null ],
        [ "Priority", "srs-qp_ao.html#srs-qp_ao-prio", null ],
        [ "Event Queue", "srs-qp_ao.html#srs-qp_ao-queue", null ],
        [ "Asynchronous Communication", "srs-qp_ao.html#srs-qp_ao-asynch", null ],
        [ "Run-to-Completion (RTC)", "srs-qp_ao.html#srs-qp_ao-rtc", null ],
        [ "Current Event", "srs-qp_ao.html#srs-qp_ao-curr", null ],
        [ "No Blocking", "srs-qp_ao.html#srs-qp_ao-block", null ],
        [ "Support For State Machines", "srs-qp_ao.html#srs-qp_ao-sm", null ],
        [ "Inversion of Control", "srs-qp_ao.html#srs-qp_ao-inv", null ],
        [ "Framework vs. Toolkit", "srs-qp_ao.html#srs-qp_ao-frame", null ],
        [ "Low Power Architecture", "srs-qp_ao.html#srs-qp_ao-power", null ]
      ] ],
      [ "Requirements", "srs-qp_ao.html#srs-qp_ao-req", [
        [ "SRS_QP_AO_00", "srs-qp_ao.html#SRS_QP_AO_00", null ],
        [ "SRS_QP_AO_01", "srs-qp_ao.html#SRS_QP_AO_01", null ],
        [ "SRS_QP_AO_10", "srs-qp_ao.html#SRS_QP_AO_10", null ],
        [ "SRS_QP_AO_11", "srs-qp_ao.html#SRS_QP_AO_11", null ],
        [ "SRS_QP_AO_20", "srs-qp_ao.html#SRS_QP_AO_20", null ],
        [ "SRS_QP_AO_21", "srs-qp_ao.html#SRS_QP_AO_21", null ],
        [ "SRS_QP_AO_22", "srs-qp_ao.html#SRS_QP_AO_22", null ],
        [ "SRS_QP_AO_23", "srs-qp_ao.html#SRS_QP_AO_23", null ],
        [ "SRS_QP_AO_30", "srs-qp_ao.html#SRS_QP_AO_30", null ],
        [ "SRS_QP_AO_31", "srs-qp_ao.html#SRS_QP_AO_31", null ],
        [ "SRS_QP_AO_32", "srs-qp_ao.html#SRS_QP_AO_32", null ],
        [ "SRS_QP_AO_40", "srs-qp_ao.html#SRS_QP_AO_40", null ],
        [ "SRS_QP_AO_50", "srs-qp_ao.html#SRS_QP_AO_50", null ],
        [ "SRS_QP_AO_51", "srs-qp_ao.html#SRS_QP_AO_51", null ],
        [ "SRS_QP_AO_60", "srs-qp_ao.html#SRS_QP_AO_60", null ],
        [ "SRS_QP_AO_70", "srs-qp_ao.html#SRS_QP_AO_70", null ]
      ] ]
    ] ],
    [ "Events", "srs-qp_evt.html", [
      [ "Concepts & Definitions", "srs-qp_evt.html#srs-qp_evt-def", [
        [ "Event Signal", "srs-qp_evt.html#srs-qp_evt-sig", null ],
        [ "Event Parameters", "srs-qp_evt.html#srs-qp_evt-par", null ]
      ] ],
      [ "Requirements", "srs-qp_evt.html#srs-qp_evt-req", [
        [ "SRS_QP_EVT_00", "srs-qp_evt.html#SRS_QP_EVT_00", null ],
        [ "SRS_QP_EVT_20", "srs-qp_evt.html#SRS_QP_EVT_20", null ],
        [ "SRS_QP_EVT_21", "srs-qp_evt.html#SRS_QP_EVT_21", null ],
        [ "SRS_QP_EVT_22", "srs-qp_evt.html#SRS_QP_EVT_22", null ],
        [ "SRS_QP_EVT_23", "srs-qp_evt.html#SRS_QP_EVT_23", null ],
        [ "SRS_QP_EVT_30", "srs-qp_evt.html#SRS_QP_EVT_30", null ],
        [ "SRS_QP_EVT_31", "srs-qp_evt.html#SRS_QP_EVT_31", null ],
        [ "SRS_QP_EVT_40", "srs-qp_evt.html#SRS_QP_EVT_40", null ],
        [ "SRS_QP_EVT_41", "srs-qp_evt.html#SRS_QP_EVT_41", null ]
      ] ]
    ] ],
    [ "State Machines", "srs-qp_sm.html", [
      [ "Concepts & Definitions", "srs-qp_sm.html#srs-qp_sm-intro", [
        [ "State", "srs-qp_sm.html#srs-qp_sm-state", null ],
        [ "Transition", "srs-qp_sm.html#srs-qp_sm-tran", null ],
        [ "State Machine", "srs-qp_sm.html#srs-qp_sm-def", null ],
        [ "Hierarchical State Machine", "srs-qp_sm.html#srs-qp_sm-hsm", null ],
        [ "State Machine Implementation Strategy", "srs-qp_sm.html#srs-qp_sm-impl", null ],
        [ "Initializing a State Machine", "srs-qp_sm.html#srs-qp_sm-init", null ],
        [ "Dispatching Events to a State Machine", "srs-qp_sm.html#srs-qp_sm-dispatch", [
          [ "State Machine Specification", "srs-qp_sm.html#srs-qp_sm-spec", null ],
          [ "State Machine Processor", "srs-qp_sm.html#srs-qp_sm-proc", null ],
          [ "Run To Completion (RTC) Processing", "srs-qp_sm.html#srs-qp_sm-rtc", null ],
          [ "Current State", "srs-qp_sm.html#srs-qp_sm-curr", null ],
          [ "Current Event", "srs-qp_sm.html#srs-qp_sm-evt", null ]
        ] ]
      ] ],
      [ "Requirements", "srs-qp_sm.html#srs-qp_sm-req", [
        [ "SRS_QP_SM_00", "srs-qp_sm.html#SRS_QP_SM_00", null ],
        [ "SRS_QP_SM_01", "srs-qp_sm.html#SRS_QP_SM_01", null ],
        [ "SRS_QP_SM_10", "srs-qp_sm.html#SRS_QP_SM_10", null ],
        [ "SRS_QP_SM_20", "srs-qp_sm.html#SRS_QP_SM_20", null ],
        [ "SRS_QP_SM_21", "srs-qp_sm.html#SRS_QP_SM_21", null ],
        [ "SRS_QP_SM_22", "srs-qp_sm.html#SRS_QP_SM_22", null ],
        [ "SRS_QP_SM_23", "srs-qp_sm.html#SRS_QP_SM_23", null ],
        [ "SRS_QP_SM_24", "srs-qp_sm.html#SRS_QP_SM_24", null ],
        [ "SRS_QP_SM_25", "srs-qp_sm.html#SRS_QP_SM_25", null ],
        [ "SRS_QP_SM_30", "srs-qp_sm.html#SRS_QP_SM_30", null ],
        [ "SRS_QP_SM_31", "srs-qp_sm.html#SRS_QP_SM_31", null ],
        [ "SRS_QP_SM_32", "srs-qp_sm.html#SRS_QP_SM_32", null ],
        [ "SRS_QP_SM_33", "srs-qp_sm.html#SRS_QP_SM_33", null ],
        [ "SRS_QP_SM_34", "srs-qp_sm.html#SRS_QP_SM_34", null ],
        [ "SRS_QP_SM_35", "srs-qp_sm.html#SRS_QP_SM_35", null ],
        [ "SRS_QP_SM_36", "srs-qp_sm.html#SRS_QP_SM_36", null ],
        [ "SRS_QP_SM_37", "srs-qp_sm.html#SRS_QP_SM_37", null ],
        [ "SRS_QP_SM_38", "srs-qp_sm.html#SRS_QP_SM_38", null ],
        [ "SRS_QP_SM_39", "srs-qp_sm.html#SRS_QP_SM_39", null ],
        [ "SRS_QP_SM_40", "srs-qp_sm.html#SRS_QP_SM_40", null ]
      ] ]
    ] ],
    [ "Event Delivery Mechanisms", "srs-qp_edm.html", [
      [ "Concepts & Definitions", "srs-qp_edm.html#srs-qp_edm-intro", [
        [ "Direct Event Posting", "srs-qp_edm.html#srs-qp_edm-post", null ],
        [ "Publish-Subscribe", "srs-qp_edm.html#srs-qp_edm-ps", null ]
      ] ],
      [ "Requirements", "srs-qp_edm.html#srs-qp_edm-req", [
        [ "SRS_QP_EDM_00", "srs-qp_edm.html#SRS_QP_EDM_00", null ],
        [ "SRS_QP_EDM_01", "srs-qp_edm.html#SRS_QP_EDM_01", null ],
        [ "SRS_QP_EDM_50", "srs-qp_edm.html#SRS_QP_EDM_50", null ],
        [ "SRS_QP_EDM_51", "srs-qp_edm.html#SRS_QP_EDM_51", null ],
        [ "SRS_QP_EDM_52", "srs-qp_edm.html#SRS_QP_EDM_52", null ],
        [ "SRS_QP_EDM_53", "srs-qp_edm.html#SRS_QP_EDM_53", null ],
        [ "SRS_QP_EDM_54", "srs-qp_edm.html#SRS_QP_EDM_54", null ],
        [ "SRS_QP_EDM_55", "srs-qp_edm.html#SRS_QP_EDM_55", null ],
        [ "SRS_QP_EDM_80", "srs-qp_edm.html#SRS_QP_EDM_80", null ],
        [ "SRS_QP_EDM_81", "srs-qp_edm.html#SRS_QP_EDM_81", null ]
      ] ]
    ] ],
    [ "Event Memory Management", "srs-qp_emm.html", [
      [ "Concepts & Definitions", "srs-qp_emm.html#srs-qp_emm-def", [
        [ "Immutable Events", "srs-qp_emm.html#srs-qp_emm-imm", null ],
        [ "Mutable Events", "srs-qp_emm.html#srs-qp_emm-mut", null ],
        [ "Automatic Event Recycling", "srs-qp_emm.html#srs-qp_emm-gc", null ],
        [ "Zero-Copy Event Management", "srs-qp_emm.html#srs-qp_emm-zero", null ],
        [ "Mutable Event Ownership Rules", "srs-qp_emm.html#srs-qp_emm-ownership", null ],
        [ "Event Pools", "srs-qp_emm.html#srs-qp_emm-pools", null ]
      ] ],
      [ "Requirements", "srs-qp_emm.html#srs-qp_emm-req", [
        [ "SRS_QP_EMM_00", "srs-qp_emm.html#SRS_QP_EMM_00", null ],
        [ "SRS_QP_EMM_10", "srs-qp_emm.html#SRS_QP_EMM_10", null ],
        [ "SRS_QP_EMM_11", "srs-qp_emm.html#SRS_QP_EMM_11", null ],
        [ "SRS_QP_EMM_20", "srs-qp_emm.html#SRS_QP_EMM_20", null ],
        [ "SRS_QP_EMM_30", "srs-qp_emm.html#SRS_QP_EMM_30", null ],
        [ "SRS_QP_EMM_40", "srs-qp_emm.html#SRS_QP_EMM_40", null ]
      ] ]
    ] ],
    [ "Time Management", "srs-qp_tm.html", [
      [ "Concepts & Definitions", "srs-qp_tm.html#srs-qp_tm-intro", [
        [ "Time Events", "srs-qp_tm.html#srs-qp_tm-te", null ],
        [ "System Clock Tick", "srs-qp_tm.html#srs-qp_tm-tick", null ],
        [ "Power Efficiency", "srs-qp_tm.html#srs-qp_tm-power", null ],
        [ "\"Tickless Mode\"", "srs-qp_tm.html#srs-qp_tm-tickless", null ],
        [ "Multiple Tick Rates", "srs-qp_tm.html#srs-qp_tm-rates", null ]
      ] ],
      [ "Requirements", "srs-qp_tm.html#srs-qp_tm-req", [
        [ "SRS_QP_TM_00", "srs-qp_tm.html#SRS_QP_TM_00", null ],
        [ "SRS_QP_TM_10", "srs-qp_tm.html#SRS_QP_TM_10", null ],
        [ "SRS_QP_TM_11", "srs-qp_tm.html#SRS_QP_TM_11", null ],
        [ "SRS_QP_TM_20", "srs-qp_tm.html#SRS_QP_TM_20", null ],
        [ "SRS_QP_TM_21", "srs-qp_tm.html#SRS_QP_TM_21", null ],
        [ "SRS_QP_TM_22", "srs-qp_tm.html#SRS_QP_TM_22", null ],
        [ "SRS_QP_TM_23", "srs-qp_tm.html#SRS_QP_TM_23", null ],
        [ "SRS_QP_TM_30", "srs-qp_tm.html#SRS_QP_TM_30", null ],
        [ "SRS_QP_TM_40", "srs-qp_tm.html#SRS_QP_TM_40", null ]
      ] ]
    ] ],
    [ "Software Tracing", "srs-qp_qs.html", [
      [ "Concepts & Definitions", "srs-qp_qs.html#srs-qp_qs-intro", [
        [ "What is Software Tracing?", "srs-qp_qs.html#srs-qp_qs-set", null ],
        [ "Software Tracing & Active Objects", "srs-qp_qs.html#srs-qp_qs-ao", null ],
        [ "QP/Spy Software Tracing System", "srs-qp_qs.html#srs-qp_qs-qpspy", null ],
        [ "Data Protocol", "srs-qp_qs.html#srs-qp_qs-proto", null ],
        [ "Run-time Filtering", "srs-qp_qs.html#srs-qp_qs-filter", null ],
        [ "Predefined Trace Records", "srs-qp_qs.html#srs-qp_qs-pre", null ],
        [ "Application-Specific Trace Records", "srs-qp_qs.html#srs-qp_qs-app", null ],
        [ "QS Dictionaries", "srs-qp_qs.html#srs-qp_qs-dict", null ],
        [ "QS-RX Receive Channel", "srs-qp_qs.html#srs-qp_qs-rx", null ],
        [ "Reentrancy", "srs-qp_qs.html#srs-qp_qs-reent", null ]
      ] ],
      [ "Requirements", "srs-qp_qs.html#srs-qp_qs-req", [
        [ "SRS_QP_QS_00", "srs-qp_qs.html#SRS_QP_QS_00", null ],
        [ "SRS_QP_QS_01", "srs-qp_qs.html#SRS_QP_QS_01", null ],
        [ "SRS_QP_QS_10", "srs-qp_qs.html#SRS_QP_QS_10", null ],
        [ "SRS_QP_QS_11", "srs-qp_qs.html#SRS_QP_QS_11", null ],
        [ "SRS_QP_QS_20", "srs-qp_qs.html#SRS_QP_QS_20", null ],
        [ "SRS_QP_QS_21", "srs-qp_qs.html#SRS_QP_QS_21", null ],
        [ "SRS_QP_QS_30", "srs-qp_qs.html#SRS_QP_QS_30", null ],
        [ "SRS_QP_QS_31", "srs-qp_qs.html#SRS_QP_QS_31", null ],
        [ "SRS_QP_QS_40", "srs-qp_qs.html#SRS_QP_QS_40", null ],
        [ "SRS_QP_QS_50", "srs-qp_qs.html#SRS_QP_QS_50", null ]
      ] ]
    ] ],
    [ "Non-Preemptive Kernel", "srs-qp_qv.html", [
      [ "Concepts & Definitions", "srs-qp_qv.html#srs-qp_qv-intro", [
        [ "QV Non-Preemptive Kernel", "srs-qp_qv.html#srs-qp_qv-def", null ],
        [ "Sharing Resources in QV", "srs-qp_qv.html#srs-qp_qv-share", null ],
        [ "Idle Processing in QV", "srs-qp_qv.html#srs-qp_qv-idle", null ],
        [ "Task-Level Response in QV", "srs-qp_qv.html#srs-qp_qv-rt", null ]
      ] ],
      [ "Requirements", "srs-qp_qv.html#srs-qp_qv-req", [
        [ "SRS_QP_QV_00", "srs-qp_qv.html#SRS_QP_QV_00", null ],
        [ "SRS_QP_QV_10", "srs-qp_qv.html#SRS_QP_QV_10", null ],
        [ "SRS_QP_QV_11", "srs-qp_qv.html#SRS_QP_QV_11", null ],
        [ "SRS_QP_QV_12", "srs-qp_qv.html#SRS_QP_QV_12", null ],
        [ "SRS_QP_QV_20", "srs-qp_qv.html#SRS_QP_QV_20", null ],
        [ "SRS_QP_QV_21", "srs-qp_qv.html#SRS_QP_QV_21", null ]
      ] ]
    ] ],
    [ "Preemptive Non-Blocking Kernel", "srs-qp_qk.html", [
      [ "Concepts & Definitions", "srs-qp_qk.html#srs-qp_qk-intro", [
        [ "QK Preemptive Non-Blocking Kernel", "srs-qp_qk.html#srs-qp_qk-def", null ],
        [ "Preemptions in QK", "srs-qp_qk.html#srs-qp_qk-pre", null ],
        [ "Single-Stack Kernel", "srs-qp_qk.html#srs-qp_qk-stack", null ],
        [ "Idle Processing in QK", "srs-qp_qk.html#srs-qp_qk-idle", null ],
        [ "Selective Scheduler Locking", "srs-qp_qk.html#srs-qp_qk-lock", null ],
        [ "Preemption-Threshold Scheduling", "srs-qp_qk.html#srs-qp_qk-pts", null ],
        [ "Task-Level Response", "srs-qp_qk.html#srs-qp_qk-rt", null ]
      ] ],
      [ "Requirements", "srs-qp_qk.html#srs-qp_qk-req", [
        [ "SRS_QP_QK_00", "srs-qp_qk.html#SRS_QP_QK_00", null ],
        [ "SRS_QP_QK_10", "srs-qp_qk.html#SRS_QP_QK_10", null ],
        [ "SRS_QP_QK_20", "srs-qp_qk.html#SRS_QP_QK_20", null ],
        [ "SRS_QP_QK_21", "srs-qp_qk.html#SRS_QP_QK_21", null ],
        [ "SRS_QP_QK_30", "srs-qp_qk.html#SRS_QP_QK_30", null ],
        [ "SRS_QP_QK_31", "srs-qp_qk.html#SRS_QP_QK_31", null ]
      ] ]
    ] ],
    [ "Preemptive Dual-Mode Kernel", "srs-qp_qxk.html", [
      [ "Concepts & Definitions", "srs-qp_qxk.html#srs-qp_qxk-intro", [
        [ "QXK Dual-Mode Kernel", "srs-qp_qxk.html#srs-qp_qxk-def", null ],
        [ "Basic Tasks", "srs-qp_qxk.html#srs-qp_qxk-basic", null ],
        [ "Extended tasks", "srs-qp_qxk.html#srs-qp_qxk-ext", null ],
        [ "QXK Blocking Mechanisms", "srs-qp_qxk.html#srs-qp_qxk-block", null ],
        [ "Selective Scheduler Locking", "srs-qp_qxk.html#srs-qp_qxk-lock", null ],
        [ "Task-Level Response", "srs-qp_qxk.html#srs-qp_qxk-rt", null ]
      ] ],
      [ "Requirements", "srs-qp_qxk.html#srs-qp_qxk-req", [
        [ "SRS_QP_QXK_00", "srs-qp_qxk.html#SRS_QP_QXK_00", null ],
        [ "SRS_QP_QXK_10", "srs-qp_qxk.html#SRS_QP_QXK_10", null ],
        [ "SRS_QP_QXK_11", "srs-qp_qxk.html#SRS_QP_QXK_11", null ],
        [ "SRS_QP_QXK_12", "srs-qp_qxk.html#SRS_QP_QXK_12", null ],
        [ "SRS_QP_QXK_13", "srs-qp_qxk.html#SRS_QP_QXK_13", null ],
        [ "SRS_QP_QXK_20", "srs-qp_qxk.html#SRS_QP_QXK_20", null ],
        [ "SRS_QP_QXK_21", "srs-qp_qxk.html#SRS_QP_QXK_21", null ],
        [ "SRS_QP_QXK_22", "srs-qp_qxk.html#SRS_QP_QXK_22", null ]
      ] ]
    ] ],
    [ "Non-Functional Requirements", "srs-qp_nf.html", [
      [ "Standards Compliance", "srs-qp_nf.html#srs-qp_nf-sta", [
        [ "SRS_QP_NF_01", "srs-qp_nf.html#SRS_QP_NF_01", null ],
        [ "SRS_QP_NF_02", "srs-qp_nf.html#SRS_QP_NF_02", null ],
        [ "SRS_QP_NF_03", "srs-qp_nf.html#SRS_QP_NF_03", null ],
        [ "SRS_QP_NF_04", "srs-qp_nf.html#SRS_QP_NF_04", null ]
      ] ],
      [ "Determinism", "srs-qp_nf.html#srs-qp_nf-det", [
        [ "SRS_QP_NF_20", "srs-qp_nf.html#SRS_QP_NF_20", null ],
        [ "SRS_QP_NF_21", "srs-qp_nf.html#SRS_QP_NF_21", null ]
      ] ],
      [ "Portability", "srs-qp_nf.html#srs-qp_nf-port", [
        [ "SRS_QP_NF_40", "srs-qp_nf.html#SRS_QP_NF_40", null ],
        [ "SRS_QP_NF_41", "srs-qp_nf.html#SRS_QP_NF_41", null ]
      ] ],
      [ "Ease of Development", "srs-qp_nf.html#srs-qp_nf-develop", [
        [ "SRS_QP_NF_50", "srs-qp_nf.html#SRS_QP_NF_50", null ]
      ] ],
      [ "Constraints", "srs-qp_nf.html#srs-qp_nf-con", [
        [ "SRS_QP_NF_10", "srs-qp_nf.html#SRS_QP_NF_10", null ],
        [ "SRS_QP_NF_11", "srs-qp_nf.html#SRS_QP_NF_11", null ],
        [ "SRS_QP_NF_12", "srs-qp_nf.html#SRS_QP_NF_12", null ],
        [ "SRS_QP_NF_13", "srs-qp_nf.html#SRS_QP_NF_13", null ],
        [ "SRS_QP_NF_14", "srs-qp_nf.html#SRS_QP_NF_14", null ]
      ] ]
    ] ]
];