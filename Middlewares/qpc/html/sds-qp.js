var sds_qp =
[
    [ "Purpose and Scope", "sds-qp.html#sds-qp_scope", [
      [ "Design Viewpoints", "sds-qp.html#sds-qp_vp", null ],
      [ "Stakeholders", "sds-qp.html#sds-qp_stake", null ],
      [ "Concerns", "sds-qp.html#sds-qp_conc", null ],
      [ "Document Conventions", "sds-qp.html#sds-qp_conv", [
        [ "Software-Design-Specification UIDs", "sds-qp.html#sds-qp_uid", null ]
      ] ],
      [ "References", "sds-qp.html#sds-qp_ref", null ]
    ] ],
    [ "Structure Viewpoint", "sds-qp_struct.html", [
      [ "Sub-Layer View", "sds-qp_struct.html#sds-qp_lay", [
        [ "SDS_QP_QEP", "sds-qp_struct.html#SDS_QP_QEP", null ],
        [ "SDS_QP_QF", "sds-qp_struct.html#SDS_QP_QF", null ]
      ] ],
      [ "Class View", "sds-qp_struct.html#sds-qp_cls", [
        [ "SDS_QP_QEvt", "sds-qp_struct.html#SDS_QP_QEvt", null ],
        [ "SDS_QP_QAsm", "sds-qp_struct.html#SDS_QP_QAsm", null ],
        [ "SDS_QP_QHsm", "sds-qp_struct.html#SDS_QP_QHsm", null ],
        [ "SDS_QP_QMsm", "sds-qp_struct.html#SDS_QP_QMsm", null ],
        [ "SDS_QP_QActive", "sds-qp_struct.html#SDS_QP_QActive", null ],
        [ "SDS_QP_QMctive", "sds-qp_struct.html#SDS_QP_QMctive", null ],
        [ "SDS_QP_QTimeEvt", "sds-qp_struct.html#SDS_QP_QTimeEvt", null ]
      ] ]
    ] ],
    [ "Interaction Viewpoint", "sds-qp_inter.html", [
      [ "Behavior View", "sds-qp_inter.html#sds-qp_inter-beh", [
        [ "SDS_QA_START", "sds-qp_inter.html#SDS_QA_START", null ]
      ] ],
      [ "Event Exchange View", "sds-qp_inter.html#sds-qp_inter-evt", [
        [ "SDS_QP_POST", "sds-qp_inter.html#SDS_QP_POST", null ],
        [ "SDS_QP_PUB", "sds-qp_inter.html#SDS_QP_PUB", null ]
      ] ]
    ] ],
    [ "State Dynamics Viewpoint", "sds-qp_state.html", [
      [ "QHsm Design View", "sds-qp_state.html#sds-qp_state-hsm", [
        [ "SDS_QA_QHsm_decl", "sds-qp_state.html#SDS_QA_QHsm_decl", null ],
        [ "SDS_QA_QHsm_top_init", "sds-qp_state.html#SDS_QA_QHsm_top_init", null ],
        [ "SDS_QA_QHsm_state", "sds-qp_state.html#SDS_QA_QHsm_state", null ],
        [ "SDS_QA_QHsm_entry", "sds-qp_state.html#SDS_QA_QHsm_entry", null ],
        [ "SDS_QA_QHsm_exit", "sds-qp_state.html#SDS_QA_QHsm_exit", null ],
        [ "SDS_QA_QHsm_nest_init", "sds-qp_state.html#SDS_QA_QHsm_nest_init", null ],
        [ "SDS_QA_QHsm_tran", "sds-qp_state.html#SDS_QA_QHsm_tran", null ],
        [ "SDS_QA_QHsm_intern", "sds-qp_state.html#SDS_QA_QHsm_intern", null ],
        [ "SDS_QA_QHsm_choice", "sds-qp_state.html#SDS_QA_QHsm_choice", null ],
        [ "SDS_QA_QHsm_hist", "sds-qp_state.html#SDS_QA_QHsm_hist", null ],
        [ "SDS_QA_QHsm_hist_tran", "sds-qp_state.html#SDS_QA_QHsm_hist_tran", null ]
      ] ],
      [ "QMsm Design View", "sds-qp_state.html#sds-qp_state-msm", [
        [ "SDS_QA_QMsm_decl", "sds-qp_state.html#SDS_QA_QMsm_decl", null ],
        [ "SDS_QA_QMsm_top_init", "sds-qp_state.html#SDS_QA_QMsm_top_init", null ],
        [ "SDS_QA_QMsm_state", "sds-qp_state.html#SDS_QA_QMsm_state", null ],
        [ "SDS_QA_QMsm_entry", "sds-qp_state.html#SDS_QA_QMsm_entry", null ],
        [ "SDS_QA_QMsm_exit", "sds-qp_state.html#SDS_QA_QMsm_exit", null ],
        [ "SDS_QA_QMsm_nest_init", "sds-qp_state.html#SDS_QA_QMsm_nest_init", null ],
        [ "SDS_QA_QMsm_tran", "sds-qp_state.html#SDS_QA_QMsm_tran", null ],
        [ "SDS_QA_QMsm_intern", "sds-qp_state.html#SDS_QA_QMsm_intern", null ],
        [ "SDS_QA_QMsm_choice", "sds-qp_state.html#SDS_QA_QMsm_choice", null ],
        [ "SDS_QA_QMsm_hist", "sds-qp_state.html#SDS_QA_QMsm_hist", null ],
        [ "SDS_QA_QMsm_hist_tran", "sds-qp_state.html#SDS_QA_QMsm_hist_tran", null ]
      ] ]
    ] ],
    [ "Time Viewpoint", "sds-qp_time.html", null ],
    [ "Algorithm Viewpoint", "sds-qp_alg.html", [
      [ "QHsm Implementation View", "sds-qp_alg.html#sds-qp_qhsm", [
        [ "SDS_QP_QHsm_ctor", "sds-qp_alg.html#SDS_QP_QHsm_ctor", null ],
        [ "SDS_QP_QHsm_init", "sds-qp_alg.html#SDS_QP_QHsm_init", null ],
        [ "SDS_QP_QHsm_dispatch", "sds-qp_alg.html#SDS_QP_QHsm_dispatch", null ],
        [ "SDS_QP_QHsm_tran-simple", "sds-qp_alg.html#SDS_QP_QHsm_tran-simple", null ],
        [ "SDS_QP_QHsm_tran-complex", "sds-qp_alg.html#SDS_QP_QHsm_tran-complex", null ]
      ] ],
      [ "QMsm Implementation View", "sds-qp_alg.html#sds-qp_qmsm", [
        [ "SDS_QP_QMsm_ctor", "sds-qp_alg.html#SDS_QP_QMsm_ctor", null ],
        [ "SDS_QP_QMsm_init", "sds-qp_alg.html#SDS_QP_QMsm_init", null ],
        [ "SDS_QP_QMsm_disp", "sds-qp_alg.html#SDS_QP_QMsm_disp", null ],
        [ "SDS_QP_QMsm_tran", "sds-qp_alg.html#SDS_QP_QMsm_tran", null ],
        [ "SDS_QP_QMsm_tat", "sds-qp_alg.html#SDS_QP_QMsm_tat", null ]
      ] ]
    ] ],
    [ "Interface Viewpoint", "sds-qp_ifc.html", [
      [ "Critical Section", "sds-qp_ifc.html#sds-qp_osal-cs", [
        [ "SDS_QP_CRIT", "sds-qp_ifc.html#SDS_QP_CRIT", null ]
      ] ],
      [ "Active Object OSAL", "sds-qp_ifc.html#sds-qp_osal-ao", null ]
    ] ]
];