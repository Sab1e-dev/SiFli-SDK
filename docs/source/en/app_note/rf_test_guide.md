# SiFli SDK RF Test Guide

Covers the finsh commands, SDK APIs, parameter values, and event handling for each test mode.

---

## 0. Test Mode Overview

Based on the test purpose, control method, and application scenario, Bluetooth RF testing falls into three major categories: **Bluetooth individual test**, **Bluetooth non-signaling test**, and **Bluetooth signaling test**. The SDK maps them to different entry points:

| Type | Purpose | Command entry | Trigger source |
|---|---|---|---|
| **BT signaling test** | Bluetooth SIG qualification (QDID), comprehensive performance and protocol conformance evaluation | `bt_cm dut` / `gap_enb_dut_mode_req()` | Over-the-air LMP_test_control from the tester |
| **BT non-signaling test** | R&D RF debugging, fast RF verification on the production line | `bt_rftest` / `bt_enter_no_signal_dut_mode()` | Local vendor HCI |
| **BLE non-signaling test** (SIG DTM) | Fast BLE RF testing on the production line, SIG-certified DTM items | `bt_rftest bletx/blerx` / `ble_enter_dut_mode()` / standard HCI (passed through via `bt_cm uart_dut`) | Local HCI commands / tester + PC over serial HCI |
| **Individual test** | Board-level in-depth RF debugging, FCC/CE regulatory pre-testing | `bt_cm uart_dut` + SiFli_RfTool | Manual, item-by-item from the PC tool |

**Signaling vs. non-signaling vs. individual:**

- **Signaling**: the instrument establishes a real Bluetooth link with the DUT and communicates bidirectionally per the standard protocol. Results are authoritative and mandatory for certification.
- **Non-signaling**: no Bluetooth connection is established; dedicated commands drive the RF chip directly into a given state (fixed channel/power, continuous TX/RX) for fast measurement. Mainly used in R&D and on the production line.
- **Individual**: an engineer sends low-level control commands to the DUT through a PC tool ([SiFli_RfTool](../tools/SiFli_RfTool/SiFli_RfTool_UM.md)), precisely controlling RF behavior (single carrier, specific modulated signals), and analyzes each item with a spectrum analyzer / power meter, also for regulatory pre-testing.

**Relationship to BQB certification**: BQB RF testing for BR/EDR uses the BT signaling path, while BQB RF testing for BLE uses SIG DTM (i.e. BLE non-signaling). BT non-signaling uses a SiFli-proprietary vendor protocol, is not part of BQB, and serves only R&D and the production line. Individual testing is not part of BQB and is commonly used for FCC/CE regulatory pre-certification and board-level in-depth tuning.

The SiFli platform provides two kinds of test firmware:

- **rf_test.bin**: the build output of SiFli's internal, dedicated RF-test project. **It is not released externally**; in special cases you need to obtain it from an FAE.
- **User.bin**: **any firmware you build yourself from this SDK** — including the example projects shipped with the SDK and the product projects you create. The SDK has integrated RF test functionality into the protocol stack, so such firmware runs the user application by default after boot and can be switched into a test state with finsh commands (`bt_cm dut` / `bt_cm uart_dut` / `bt_rftest`), **with no need to request anything from an FAE**.

This document focuses on **User.bin**.

---

## 1. Build Configuration

### 1.1 Do the three test types require configuration changes

The BT example projects shipped with the SDK enable `CONFIG_BT_FINSH` and `CONFIG_BSP_BT_CONNECTION_MANAGER` by default, but `CONFIG_BT_RF_TEST` is **disabled** by default. The configuration threshold differs across the three test types:

| Test scenario | Command entry | proj.conf change needed? |
|---|---|---|
| Signaling test | `bt_cm dut` / `gap_enb_dut_mode_req()` | **No** — BT examples work out of the box |
| Individual test (SiFli_RfTool pass-through) | `bt_cm uart_dut` | **Yes**, `CONFIG_BT_RF_TEST=y`. The command itself is not gated by this switch and will not error if it is missing, but the UART will not switch to HCI pass-through and SiFli_RfTool will receive no response (this matches the "no HCI response after `bt_cm uart_dut`" row in §14) |
| Non-signaling test | `bt_rftest enter/bttx/btrx/bletx/blerx` | **Yes**, `CONFIG_BT_RF_TEST=y`. The entire `bt_rftest` command is gated by this switch; without it, the command simply does not exist in finsh |

For a project you build yourself, or for testing in a project where the BT protocol stack is not enabled, check each item against the §1.3 table (`CONFIG_RT_USING_FINSH`, `CONFIG_BSP_BT_CONNECTION_MANAGER`, and, where needed, `CONFIG_BT_FINSH` / `CONFIG_BT_RF_TEST`).

### 1.2 Recommended base projects

If you don't want to build a project from scratch, the following SDK example projects can be used directly as a base with minimal changes:

| Test scenario | Recommended base project | Changes |
|---|---|---|
| BT signaling test (BR/EDR DUT mode) | Any BT example under `example/bt/`, e.g. [`example/bt/spp/`](../../../../example/bt/spp/) | Compile directly; no `proj.conf` change needed |
| BT non-signaling / individual pass-through (`bt_cm uart_dut` + SiFli_RfTool) | Same as above | Append `CONFIG_BT_RF_TEST=y` to the end of `project/proj.conf` and rebuild |
| BLE DTM (`bt_rftest bletx/blerx`) | Same as above (BT examples include the BLE controller by default) | Same as above, add `CONFIG_BT_RF_TEST=y` |
| Tester/PC script driving standard HCI directly (pass-through from boot, no runtime finsh switching) | [`example/bt/HCI_over_uart/`](../../../../example/bt/HCI_over_uart/) | No `proj.conf` change; but this example has no `bt_cm` command and no BLE/BT application — it runs only standard HCI |

> Note: `example/bt/HCI_over_uart/` works in a fundamentally different way from the three rows above — it **hands UART1 to the controller at boot**, the firmware itself does not run an application state, and it neither needs nor supports `bt_cm uart_dut`. The other three rows are all based on application-state firmware plus runtime finsh command switching.

### 1.3 Kconfig switch summary

| Command / feature | Required Kconfig | Code location |
|---|---|---|
| `bt_cm` command | `CONFIG_RT_USING_FINSH=y` + `CONFIG_BSP_BT_CONNECTION_MANAGER=y` | `bt_connection_manager.c:1937` |
| `bt_rftest` command | `CONFIG_RT_USING_FINSH=y` + `CONFIG_BT_RF_TEST=y` (and not SF32LB55X) | `bf0_bt_common.c:1209` |
| The **UART switching part** inside `bt_cm uart_dut` | `CONFIG_BT_RF_TEST=y` (the command itself is not gated by this switch; without it, `uart_ipc_path_change` simply is not called) | `bt_connection_manager.c:1776` |
| The `<< enb DUT mode` and other BT app traces in §12.1 | `CONFIG_BT_FINSH=y` | `bt_finsh/bts2_app_generic.c` |

> `BT_FINSH` is the master switch for BT protocol-stack debug traces (the `bts2_app_demo` / `bts2_app_menu` / `bts2_app_generic` set); do not treat it as the switch that "enables bt_cm / bt_rftest".

### 1.4 Target device for UART path switching

Applies only to `bt_cm uart_dut` and to custom entry points that call `uart_ipc_path_change()`:

```
# The default is IPC_USE_CONSOLE_DEVICE=y; the console UART runs finsh and is the one that gets switched
# To switch a different UART (e.g. in a custom product, pointing the test port at uart2), change to:
CONFIG_IPC_USE_OWN_DEVICE=y
CONFIG_IPC_OWN_DEVICE_NAME="uart2"
```

---

## 2. BT Classic Signaling Test

### 2.1 Topology

The relationship between the DUT, the comprehensive tester, and the PC control side:

```{figure} assert/bt_signaling_test.png
:align: center

BT signaling test topology
```

The comprehensive tester (MT88 series, CMW series, etc.) acts as the master device over the air interface and drives the DUT into the signaling test state via LMP commands; the PC controls the tester over serial/GPIB. The DUT itself only needs to set the DUT enable flag at the protocol-stack level.

### 2.2 How to use

| Option | Call |
|---|---|
| FinSH | `bt_cm dut` (sets the enable flag only) / `bt_cm uart_dut` (sets the enable flag + switches the UART) |
| API | `gap_enb_dut_mode_req(U16 tid)`, header `gap_api.h` |

For detailed test procedures, see [SiFli SF32LB5xx Bluetooth Signaling Test Guide](bt_signaling_test_guide.md).

### 2.3 Procedure

```
1. The protocol stack starts normally; BLE_POWER_ON_IND has been received
2. The host explicitly enables scan (required):
     bt_interface_set_scan_mode(1, 1);
     // or gap_wr_scan_enb_req(tid, 1, 1);
   gap_enb_dut_mode_req does not automatically enable inquiry/page scan; if the host doesn't, the tester cannot find the DUT.
3. Call gap_enb_dut_mode_req(bts2_task_get_app_task_id()):
   - issues HCI_Enable_Device_Under_Test_Mode
   - the controller sets the "allow DUT" flag and afterwards accepts over-the-air LMP_test_control
4. Success is indicated when the console shows "<< enb DUT mode : <step_num>" without "<< failed to enb DUT mode!"
5. The instrument (CMW500 in signaling mode) connects over the air:
   inquiry -> page -> ACL connection -> LMP_test_control -> enters the test state
6. The instrument runs the test suite
```

### 2.4 Complete code example

```c
#include "bts2_app_inc.h"  /* contains gap_api.h */

void enter_bt_signaling_dut(void)
{
    gap_enb_dut_mode_req(bts2_task_get_app_task_id());
    /* Optionally wait for the BTS2MU_GAP_ENB_DUT_MODE_CFM event */
}
```

In the event callback (inside the BT event handler you registered):

```c
case BTS2MU_GAP_ENB_DUT_MODE_CFM:
{
    BTS2S_GAP_ENB_DUT_MODE_CFM *cfm = (BTS2S_GAP_ENB_DUT_MODE_CFM *)data;
    if (cfm->res == 0)
        LOG_I("DUT mode enabled");
    else
        LOG_E("DUT mode enable failed: %d", cfm->res);
    break;
}
```

### 2.5 Notes

- **It only "enables"** — it does not make the device transmit on its own. Actual transmission begins only when the instrument triggers it via LMP_test_control over the air.
- **The HCI command channel remains fully functional**; afterwards you can still send HCI Reset, Read BD_Addr, and LE DTM commands.
- **BLE is completely unaffected**; the BLE controller works as usual.
- **The only way to exit is a reset**; there is no inverse operation.
- Preconditions before calling: `sifli_ble_enable()` has completed, `BLE_POWER_ON_IND` has arrived, and the current operating mode includes BT Classic (dual-mode or BT-only, i.e. the BT protocol stack is loaded).

---

## 3. BT Classic Non-Signaling Test

### 3.1 Topology

A non-signaling test does not establish a Bluetooth link with the DUT; the PC controls the DUT by sending HCI commands over serial, while controlling the comprehensive tester over serial or GPIB at the same time:

```{figure} assert/non_signaling_test.png
:align: center

Non-signaling test topology
```

### 3.2 How to use

| Option | Call |
|---|---|
| FinSH | `bt_rftest enter` -> `bt_rftest bttx/btrx` -> `bt_rftest btstop` -> `bt_rftest exit` |
| API | `bt_enter_no_signal_dut_mode(bt_ns_test_mode_ctrl_cmd_t *)` |

Header: `bf0_ble_common.h`

### 3.3 Operation codes and parameters

```c
enum bt_test_operation_t {
    BT_TEST_OP_ENTER_TEST,    // enter the non-signaling test state
    BT_TEST_OP_EXIT_TEST,     // exit
    BT_TEST_OP_TX_TEST,       // start transmitting (parameters via tx_para)
    BT_TEST_OP_RX_TEST,       // start receiving (parameters via rx_para)
    BT_TEST_OP_STOP_TEST,     // stop the current TX/RX
};

typedef struct {
    uint8_t  channel;     // 0..78, BT channel
    uint8_t  pkt_payload; // 0=PRBS9, 1=11110000, 2=10101010, 3=PRBS15, ...
    uint8_t  pkt_type;    // 0=ID, 1=NULL, 4=DM1, 5=DH5 (common), 14=2-DH5, 15=3-DH5
    uint8_t  pwr_lvl;     // dBm as a plain integer (typically 0..13); quantized at runtime to a hardware level supported by the chip PA, see §7.1
    uint16_t pkt_len;     // payload length, DH5 max 339
} bt_ns_test_mode_tx_para_t;

typedef struct {
    uint8_t channel;      // 0..78
    uint8_t pkt_type;     // same as above
} bt_ns_test_mode_rx_para_t;
```

### 3.4 FinSH flow example

```
msh > bt_rftest enter           # enter the non-signaling test state
msh > bt_rftest bttx            # ch8, DH5, 339B, PRBS9, pwr=10 (fixed parameters)
... the tester measures TX power and frequency offset (10-30 s) ...
msh > bt_rftest btstop          # stop TX
msh > bt_rftest btrx            # ch0, DH1 receive
... the tester transmits from the far end, the DUT computes PER ...
msh > bt_rftest btstop
msh > bt_rftest exit            # exit the test state
```

The test parameters of the `bt_rftest` subcommands (channel, packet type, length, power) are fixed values inside the SDK. To adjust them, refer to the `bt_rftest()` implementation in `SiFli-SDK/middleware/bluetooth/service/common/bf0_bt_common.c` and define your own parameterized subcommands.

### 3.5 Complete API usage example

> **`bt_enter_no_signal_dut_mode` is asynchronous.** Internally it does `sifli_msg_alloc` + `memcpy` + `sifli_msg_send` and returns `HL_ERR_NO_ERROR` immediately; it does not actually take effect until the `BT_NS_DUT_RSP` event acknowledges it (implementation in `bf0_bt_common.c:793`). The demo below uses event waits between state transitions to guarantee ordering; if your production code must proceed synchronously, you can release an `rt_event` / `rt_sem` in the `BT_NS_DUT_RSP` callback, or, as a last resort, use an empirical delay (typically 100-200 ms is enough, but not guaranteed).

```c
#include "bf0_ble_common.h"

/* In the BT event handler, receive BT_NS_DUT_RSP and release the waiter (pseudocode) */
static rt_event_t s_bt_ns_evt;       /* created with rt_event_create during module init */
#define EVT_BT_NS_DONE  (1u << 0)

static void on_bt_ns_dut_rsp(bt_ns_test_mode_ctrl_rsp_t *rsp)
{
    /* You can dispatch further based on rsp->op / rsp->status */
    rt_event_send(s_bt_ns_evt, EVT_BT_NS_DONE);
}

static int wait_bt_ns_done(uint32_t timeout_ms)
{
    rt_uint32_t recved;
    return rt_event_recv(s_bt_ns_evt, EVT_BT_NS_DONE,
                         RT_EVENT_FLAG_OR | RT_EVENT_FLAG_CLEAR,
                         rt_tick_from_millisecond(timeout_ms), &recved);
}

void bt_nonsig_tx_test(uint8_t channel, uint16_t pkt_len, uint8_t pwr_lvl)
{
    bt_ns_test_mode_ctrl_cmd_t cmd;

    /* 1. Enter the test state, wait for the ack */
    cmd.op = BT_TEST_OP_ENTER_TEST;
    bt_enter_no_signal_dut_mode(&cmd);
    if (wait_bt_ns_done(500) != RT_EOK) {
        LOG_E("ENTER timeout"); return;
    }

    /* 2. Configure TX parameters and start */
    cmd.op = BT_TEST_OP_TX_TEST;
    cmd.para.tx_para.channel     = channel;
    cmd.para.tx_para.pkt_type    = 5;       /* DH5 */
    cmd.para.tx_para.pkt_len     = pkt_len;
    cmd.para.tx_para.pkt_payload = 0;       /* PRBS9 */
    cmd.para.tx_para.pwr_lvl     = pwr_lvl;
    bt_enter_no_signal_dut_mode(&cmd);
    /* The ack for TX/RX start is also reported via BT_NS_DUT_RSP; a production flow should likewise wait for it */
}

void bt_nonsig_stop_and_exit(void)
{
    bt_ns_test_mode_ctrl_cmd_t cmd;

    cmd.op = BT_TEST_OP_STOP_TEST;
    bt_enter_no_signal_dut_mode(&cmd);
    wait_bt_ns_done(500);

    cmd.op = BT_TEST_OP_EXIT_TEST;
    bt_enter_no_signal_dut_mode(&cmd);
    wait_bt_ns_done(500);
}
```

### 3.6 Receive results (RX only)

When the `BT_NS_DUT_RSP` event arrives, the corresponding struct is `bt_ns_test_mode_ctrl_rsp_t`:

```c
typedef struct {
    enum bt_test_operation_t  op;        // on STOP_TEST, returns cnt
    uint8_t                    status;
    bt_ns_test_mode_rsp_para_t para;     // .stop_para.cnt = total packets received
} bt_ns_test_mode_ctrl_rsp_t;
```

Or use the newer synchronous interface to get PER + RSSI directly:

```c
extern int8_t bt_ns_rx_start(bt_ns_test_new_rx_para_t *rxpara,
                              bt_ns_test_new_rx_rslt_t *rst,
                              uint32_t delay_ms);

bt_ns_test_new_rx_para_t para = {
    .channel = 8, .pkt_type = 5, .pkt_len = 339, .pkt_payload = 0,
};
bt_ns_test_new_rx_rslt_t rst;
bt_ns_rx_start(&para, &rst, 2000);  /* receive for 2 s, then stop */
LOG_I("err_bit=%d total_bit=%d err_pkt=%d total_pkt=%d rssi=%d",
      rst.err_bit_num, rst.total_bit_num,
      rst.err_pkt_num, rst.total_pkt_num, rst.rssi);
```

### 3.7 Notes

- **The comprehensive tester does not recognize** this command (SiFli-proprietary vendor); use **[SiFli_RfTool](../tools/SiFli_RfTool/SiFli_RfTool_UM.md)**.
- **You must ENTER before TX/RX, STOP before switching between TX and RX, and EXIT last.** An incorrect state machine will cause errors.

---

## 4. BLE Non-Signaling Test (SIG DTM)

### 4.1 Topology

All BLE RF testing is done by sending 3 HCI commands (`LE_Transmitter_Test` / `LE_Receiver_Test` / `LE_Test_End`) over the serial link between the DUT and the comprehensive tester. On the firmware side there is only this one path: switching the UART to HCI pass-through:

```{figure} assert/ble_signaling_test.png
:align: center

BLE DTM test topology
```

The DUT and the comprehensive tester are connected over serial; the tester sends HCI commands to drive the DUT into TX/RX. The RF test tool on the PC controls both the DUT's and the tester's transmit/receive at the same time.

### 4.2 How to use

| Option | Call |
|---|---|
| FinSH | `bt_rftest bletx` / `bt_rftest blerx` / `bt_rftest btstop` (shared stop) / `bt_rftest exit` |
| API | `ble_enter_dut_mode(ble_dut_mode_t *)` |
| Standard HCI (via UART pass-through) | `HCI_LE_Transmitter_Test` (0x201E) / `HCI_LE_Receiver_Test v2` (0x2033) / `HCI_LE_Test_End` (0x201F) |

Header: `bf0_ble_common.h`

### 4.3 Data structures

```c
typedef struct {
    uint8_t operation;       // GAPM_LE_TEST_TX_START / RX_START / STOP
    uint8_t channel;         // 0..39 (0=2402MHz, 19=2440MHz, 39=2480MHz)
    uint8_t tx_data_length;  // TX only, 0..255
    uint8_t tx_pkt_payload;  // TX only, gap_pkt_pld_type
    uint8_t phy;             // 1=1M, 2=2M, 3=Coded
    uint8_t modulation_idx;  // RX only, 0=standard, 1=stable
} ble_dut_mode_t;

enum gap_pkt_pld_type {
    GAP_PKT_PLD_PRBS9                = 0,  // default, common on the production line
    GAP_PKT_PLD_REPEATED_11110000    = 1,  // for measuring modulation Δf1/Δf2
    GAP_PKT_PLD_REPEATED_10101010    = 2,  // for measuring carrier frequency offset
    GAP_PKT_PLD_PRBS15               = 3,
    /* also all-0, all-1, F0/0F, AA/55, etc. */
};
```

### 4.4 Sending directly via the API

> **Both `bt_enter_no_signal_dut_mode` and `ble_enter_dut_mode` are asynchronous** (implementations in `bf0_bt_common.c:769` / `:793`). The two commands are posted to two different task queues, COMMON and GAPM respectively, so **you cannot assume they complete serially in call order**. Completion events:
> - ENTER done -> `BT_NS_DUT_RSP` (`op == BT_TEST_OP_ENTER_TEST`)
> - TX start done -> `BLE_DUT_TX_START_CNF`
> - RX start done -> `BLE_DUT_RX_START_CNF`
> - Test end (STOP) -> `BLE_DUT_END_IND` (includes `nb_packet_received`)
>
> Production/product code must wait for the corresponding event before moving to the next step. The demo below uses `rt_event` to chain the entire sequence.

```c
#include "bf0_ble_common.h"

static rt_event_t s_dut_evt;
#define EVT_NS_ENTER_DONE   (1u << 0)
#define EVT_BLE_TX_START    (1u << 1)
#define EVT_BLE_END         (1u << 2)

/* In the BT/BLE event handler, set the corresponding bit */
static void on_bt_ns_dut_rsp(bt_ns_test_mode_ctrl_rsp_t *rsp) {
    if (rsp->op == BT_TEST_OP_ENTER_TEST)
        rt_event_send(s_dut_evt, EVT_NS_ENTER_DONE);
}
static void on_ble_dut_tx_cnf(ble_test_tx_start_cnf_t *cnf) {
    rt_event_send(s_dut_evt, EVT_BLE_TX_START);
}
static void on_ble_dut_end(ble_dut_end_ind_t *ind) {
    LOG_I("DTM end, nb_packet_received=%u", ind->nb_packet_received);
    rt_event_send(s_dut_evt, EVT_BLE_END);
}

static int wait_evt(uint32_t bits, uint32_t timeout_ms)
{
    rt_uint32_t recved;
    return rt_event_recv(s_dut_evt, bits,
                         RT_EVENT_FLAG_OR | RT_EVENT_FLAG_CLEAR,
                         rt_tick_from_millisecond(timeout_ms), &recved);
}

/* 1. Enter the test state (BT_TEST_OP_ENTER_TEST is the shared BLE/BT entry) */
bt_ns_test_mode_ctrl_cmd_t enter = { .op = BT_TEST_OP_ENTER_TEST };
bt_enter_no_signal_dut_mode(&enter);
if (wait_evt(EVT_NS_ENTER_DONE, 500) != RT_EOK) {
    LOG_E("ENTER timeout"); return;
}

/* 2. Start TX: ch19 (2440 MHz), 37 bytes, PRBS9, 1M PHY */
ble_dut_mode_t tx = {
    .operation       = GAPM_LE_TEST_TX_START,
    .channel         = 19,
    .tx_data_length  = 37,
    .tx_pkt_payload  = GAP_PKT_PLD_PRBS9,
    .phy             = 1,
    .modulation_idx  = 0,
};
ble_enter_dut_mode(&tx);
if (wait_evt(EVT_BLE_TX_START, 500) != RT_EOK) {
    LOG_E("TX start timeout"); return;
}

/* 3. Let the controller transmit continuously for 2 s, then stop */
rt_thread_mdelay(2000);
ble_dut_mode_t stop = { .operation = GAPM_LE_TEST_STOP };
ble_enter_dut_mode(&stop);
wait_evt(EVT_BLE_END, 500);   /* Receiving BLE_DUT_END_IND indicates it has stopped */
```

### 4.5 Running via FinSH (parameters fixed to ch0)

```
msh > bt_rftest enter
msh > bt_rftest bletx           # ch0, default 37B PRBS9 1M PHY
... the tester measures ...
msh > bt_rftest btstop
msh > bt_rftest exit
```

`bt_rftest blerx` works the same way. After RX completes, `nb_packet_received` is returned via the `BLE_DUT_END_IND` event.

### 4.6 Via HCI pass-through (driven directly by the tester)

If you use a standard instrument such as the CMW500, skip the API and send HCI directly:

```
1. msh > bt_cm uart_dut       # switch the UART to HCI pass-through
2. afterwards the instrument/script sends standard HCI commands over this UART:

   01 1E 20 03 13 25 00       LE_Transmitter_Test    ch19, 37B, PRBS9
   01 33 20 03 13 01 00       LE_Receiver_Test v2    ch19, 1M PHY, modulation=standard
   01 1F 20 00                LE_Test_End -> returns packet_count

3. exit: reset the chip, or call `uart_ipc_path_revert()` to switch back in software (see §11.5)
```

### 4.7 Event callbacks

```c
case BLE_DUT_TX_START_CNF: {
    ble_test_tx_start_cnf_t *cnf = (ble_test_tx_start_cnf_t *)data;
    LOG_I("TX start, status=%d", cnf->status);
    break;
}
case BLE_DUT_RX_START_CNF: {
    ble_test_rx_start_cnf_t *cnf = (ble_test_rx_start_cnf_t *)data;
    LOG_I("RX start, status=%d", cnf->status);
    break;
}
case BLE_DUT_END_IND: {
    ble_dut_end_ind_t *ind = (ble_dut_end_ind_t *)data;
    LOG_I("Test end, packet_count=%d", ind->nb_packet_received);
    /* In TX mode packet_count is always 0; this is spec behavior */
    break;
}
```

### 4.8 Notes

- **No `gap_enb_dut_mode_req` needed**: BLE DTM is non-signaling and does not require setting the BT Classic "allow" flag.
- **In a TX test packet_count is always 0** (per spec); it is meaningful only for RX.
- BLE and BT Classic share the radio and **cannot run in parallel**.

---

## 5. Individual Test (SiFli_RfTool)

Used for board-level RF tuning during R&D and for regulatory pre-testing. An engineer sends low-level control commands to the DUT's serial port through **[SiFli_RfTool](../tools/SiFli_RfTool/SiFli_RfTool_UM.md)** on the PC, precisely controlling RF behavior (single carrier, specific modulated signals, fixed channel/power), and measures each item with instruments such as a spectrum analyzer or power meter.

**Tool location**: `tools/SiFli_RfTool/SiFli_RfTool.exe`, shipped with the SDK, runs without installation (Windows), **no need to request it from an FAE**.

For detailed individual-test steps, see [SiFli SF32LB5xx Bluetooth Individual RF Test Guide](bt_individual_test_guide.md).

### 5.1 Entry points

| Option | Call |
|---|---|
| FinSH | `bt_cm uart_dut` (sets the DUT enable flag + switches the UART to HCI pass-through) |
| API | Same as Chapter 2 (`gap_enb_dut_mode_req` + `uart_ipc_path_change`) |

Once the firmware side has switched, SiFli_RfTool sends low-level HCI / vendor commands over serial to control each item. The firmware itself needs no extra logic; it only has to hand over the UART path.

### 5.2 Usage

On the DUT, run `bt_cm uart_dut` -> open SiFli_RfTool on the PC -> select the serial port and baud rate -> send commands item by item from the tool's UI to test. The serial commands can also be issued with **SiFli_Trace** or another general-purpose serial tool (common in signaling test mode).

---

## 6. RF Debug Serial Port and Baud Rate

Default RF debug serial port and pin mapping per chip (default baud rate **1000000**):

| Chip model | Interface type | Pins | UART channel (TX / RX) | Notes |
|---|---|---|---|---|
| SF32LB551 | RF debug serial port | PA49, PA51 | UART1: TX(PA49) / RX(PA51) | |
| SF32LB551 | Single-carrier test | PB45, PB46 | UART3: TX(PB45) / RX(PB46) | |
| SF32LB555 | RF debug serial port | PA17, PA19 | UART1: TX(PA17) / RX(PA19) | |
| SF32LB555 | Single-carrier signal port | PB45, PB46 | UART3: TX(PB45) / RX(PB46) | |
| SF32LB561 / SF32LB563 | RF debug serial port | PA17, PA18 | UART1: TX(PA17) / RX(PA18) | |
| SF32LB567 | RF debug serial port | PA34, PA30 | UART1: TX(PA34) / RX(PA30) | |
| SF32LB52x | RF debug serial port | PA19, PA18 | UART1: TX(PA19) / RX(PA18) | Shares pins with the software download port |
| SF32LB58x | RF debug serial port | PA31, PA32 | UART1: TX(PA31) / RX(PA32) | |

> For historical reasons, the SF32LB55X series requires switching to UART3 for the **single-carrier test**. On all other chips, all RF testing is unified on UART1.

The comprehensive tester (MT88 series, CMW series) must match this baud rate (1 Mbps by default) in its HCI-over-UART configuration. If the link is unstable, you can drop to 921600 / 460800 / 115200 and lower the tester side to match.

---

## 7. RF Power Levels and Configuration

### 7.1 Supported power levels

The power levels the chip PA can output stably (other dBm values are internally quantized to the nearest level):

| Level | Notes |
|---|---|
| 0 dBm | |
| 3 dBm | |
| 6 dBm | |
| 9 dBm | |
| 13 dBm | |
| 19 dBm | **Only BR & BLE support 19 dBm** |

> The software APIs (`pwr_lvl`, the Kconfig `BT_TX_POWER_VAL_*`) accept any integer dBm value, not a level index; the controller quantizes it at runtime to the nearest hardware level.

### 7.2 menuconfig configuration

```
(Top) -> SiFli SDK configuration -> Board Config -> Select BT RF TX power
```

The Kconfig fields are all of **`int` type — enter the dBm value directly as an integer** (see [customer/boards/Kconfig:103+](D:/OpenSiFli/SiFli-SDK/customer/boards/Kconfig#L103)). The value range on the new platforms (52X/56X/58X) is 0-13 dBm; on the older platform, the BLE range is -10 to 10 dBm.

| Option | Meaning |
|---|---|
| Select BLE MAX TX power | BLE maximum transmit power (dBm integer) |
| Select MINIMUM TX power | BR/EDR/BLE minimum transmit power (dBm integer) |
| Select classic BT MAX TX power | BR/EDR maximum transmit power (dBm integer); if this value is smaller than the BLE maximum, the BR/EDR maximum follows the BLE value, otherwise this value is used |

The `pwr_lvl` field of the non-signaling test TX command has the same semantics as these Kconfig fields: **a plain dBm value, not a level index**.

---

## 8. HCI Command Quick Reference

Below is an overview of the common HCI commands related to BT/BLE RF testing:

```{figure} assert/hci_cmd.png
:align: center

HCI command set
```

Some key opcodes:

| Command | OGF/OCF | Full opcode (LE) |
|---|---|---|
| HCI Reset | 0x03 / 0x0003 | `01 03 0C 00` |
| HCI Read BD_Addr | 0x04 / 0x0009 | `01 09 10 00` |
| HCI Enable Device Under Test Mode | 0x06 / 0x0003 | `01 03 18 00` |
| LE Receiver Test v2 | 0x08 / 0x0033 | `01 33 20 03 ch phy mod` |
| LE Transmitter Test | 0x08 / 0x001E | `01 1E 20 03 ch len pld` |
| LE Test End | 0x08 / 0x001F | `01 1F 20 00` |

For more detailed command parameters and response fields, see the [Bluetooth Core Specification](https://www.bluetooth.com/specifications/specs/) Volume 4 Part E (HCI). The SiFli-proprietary vendor commands (used for BT non-signaling testing) are not part of the SIG standard; they are wrapped inside the APIs in Chapter 3, so you do not need to assemble the bytes yourself.

---

## 9. The Three Test States Share the Radio and Cannot Run in Parallel

The LCPU has only one RF front end, so it can be in only one test state at any time:

```
Normal operating state  --[gap_enb_dut_mode_req]-->  BT signaling DUT ready state
                        --[bt_enter_no_signal_dut_mode(ENTER+TX)]-->  BT non-signaling test state
                        --[ble_enter_dut_mode(TX_START)]-->  BLE DTM test state
```

**Switching rules**:

- Before entering any test state, if you were previously in another test state, you must EXIT cleanly first
- BT signaling DUT and BT non-signaling are mutually exclusive; EXIT before switching
- BLE DTM and BT non-signaling are mutually exclusive (they share the ENTER state); EXIT then ENTER before switching

---

## 10. Custom Test Entry in Your Product

If you want to add the ability to "trigger RF testing via a proprietary protocol command" in your own product (for example, a production-line station sends one command and the module enters DUT), refer to the two templates below.

**Template A: set the BT Classic DUT enable flag, leave the UART untouched. Exit by reset.**

```c
void product_enter_bt_signaling_dut(void)
{
    gap_enb_dut_mode_req(bts2_task_get_app_task_id());
    /* The application UART keeps working; the instrument connects over the air to trigger the test */
}
```

**Template B: switch the application UART into HCI H4 pass-through for use by the CMW500.**

After this, the calling thread **must not return to the main loop and access this UART**, otherwise the next `rt_device_read(NULL)` will assert. You must release the UART and permanently suspend the current thread:

```c
extern void uart_ipc_path_change(void);

void product_enter_hci_passthrough(void)
{
    /* 1. Send an ACK or similar reply to the host, then wait for the TX to drain */
    rt_thread_mdelay(50);

    /* 2. Release the application UART: stop the receive thread, unbind rx_indicate, close the device */
    my_app_release_uart();

    /* 3. The SDK takes over this UART and starts the fwd2mb / fwd2uart forwarding threads */
    uart_ipc_path_change();

    /* 4. Permanently suspend the current thread so it doesn't access the now-closed UART after returning */
    rt_thread_suspend(rt_thread_self());
    rt_schedule();
    /* never returns */
}
```

What `my_app_release_uart()` must do:

- Make your receive thread aware that it should exit (a flag or semaphore)
- `rt_device_set_rx_indicate(dev, RT_NULL)`
- `rt_device_close(dev)`

The transmit APIs (your product's own `xxx_uart_send`) must include a NULL check after release, otherwise subsequent event reporting will trigger a null-pointer dereference.

### 10.1 Where to hook the command handler

The hook point depends on your product's command system:

| Command system | How to hook |
|---|---|
| Standard finsh / MSH | Export directly with `MSH_CMD_EXPORT(my_dut_cmd, ...)` |
| Custom text protocol (AT, proprietary ASCII, etc.) | Add it to the command dispatch table of your protocol parser; the handler runs in the protocol-parser thread |
| Binary proprietary protocol | Add it to the opcode dispatch switch; the handler runs in the protocol-parser thread |
| BLE/SPP remote trigger | Call it inside the GATT write callback or the SPP data callback |

**Mind the handler's thread context**: the `rt_thread_suspend(rt_thread_self())` in Template B suspends **the thread that runs the `uart_ipc_path_change()` call**. If your handler runs in the protocol-parser thread (the typical case), that is the thread being suspended, and all subsequent processing logic on that thread will no longer run — this is expected behavior.

If you don't want the entire protocol-processing thread frozen, you can **spawn a temporary one-shot thread** dedicated to running the Template B logic and suspend in that temporary thread; but this brings little benefit, because once the UART has been switched the protocol-parser thread has nothing left to do (the UART no longer belongs to it), so it makes no difference which thread is suspended.

### 10.2 Calling preconditions

Both Template A and Template B require the **BT protocol stack to be ready**:

- `sifli_ble_enable()` has been called, **and** the `BLE_POWER_ON_IND` event has arrived
- When the current operating mode includes BT Classic (dual-mode or BT-only), `bt_interface_set_scan_mode(1, 1)` has taken effect

If these steps complete asynchronously in your product's power-on flow, **the handler must guard against it**:

```c
extern app_env_t *app_get_env(void);   /* however your product tracks stack state */

static void product_enter_dut(void)
{
    if (!app_get_env()->is_power_on)
    {
        /* the stack isn't up yet; calling now would go nowhere */
        respond_error("stack not ready");
        return;
    }
    gap_enb_dut_mode_req(bts2_task_get_app_task_id());
    respond_ok();
}
```

---

## 11. Data-Path Changes After HCI Pass-Through

After Template B calls `uart_ipc_path_change()`, be clear about what changes on that UART, or you'll easily run into trouble:

### 11.1 What happens

| Before the switch | After the switch |
|---|---|
| RX data is consumed by your protocol-parser thread | RX data is read by the SDK's `fwd2mb` thread and sent straight to the LCPU via mailbox |
| TX goes through your protocol code's `rt_device_write` | TX is written by the SDK's `fwd2uart` thread, taking data from the mailbox |
| `rx_indicate` is your callback | `rx_indicate` is replaced by the SDK with `hci_trans_uart_rx_ind` |
| log output goes via ulog -> console UART (possibly this one) | if the console UART was switched, `log_pause(true)` has been called and logging is silenced |
| the UART carries a text / proprietary protocol | the UART carries an **HCI H4 binary stream** |

### 11.2 What you must avoid after switching

- **Do not call any of your previous `xxx_uart_send` / `xxx_uart_send_str` functions**: even with a NULL check to prevent a crash, if those bytes are mistakenly written to the UART, the SDK will treat them as an HCI command and forward them to the LCPU. After parsing them as H4 frames, the LCPU will get an illegal opcode or illegal length — at best it returns `Unknown HCI Command (0x01)` / `Invalid HCI Parameters (0x12)`; at worst the byte stream of a following legitimate HCI command is truncated by the `param_len` of the previous bad frame, causing command loss or misaligned responses
- **Do not trigger any log printing to this UART**: `log_pause(true)` has paused the ulog framework, but if you bypass ulog and call `rt_kprintf` directly, the bytes will leak into the HCI stream and corrupt the host
- **Do not report any more events to this UART**: periodic output such as disconnect/connect events and heartbeats must be stopped

### 11.3 What you can do after switching

- **Receive HCI commands on this UART** (sent by the instrument / PC script; opcode encoding matches the §8 table, with `OGF/OCF` combined into a 16-bit value):
  - Standard BLE DTM: `HCI_LE_Transmitter_Test` (`0x08/0x001E`), `HCI_LE_Receiver_Test v2` (`0x08/0x0033`), `HCI_LE_Test_End` (`0x08/0x001F`)
  - Generic HCI: `HCI_Reset` (`0x03/0x0003`), `HCI_Read_BD_Addr` (`0x04/0x0009`), etc.
  - SiFli-proprietary BT non-signaling vendor: see Chapter 3
- **Observe HCI events on this UART**: each command should return a Command Complete event (type `0x04`, event code `0x0E`)

### 11.4 H4 frame format quick reference

Send (host -> controller):

```
| type | opcode_lo | opcode_hi | param_len | params... |
| 0x01 |  XX       |    XX     |    XX     |   ....    |
```

Receive (controller -> host):

```
| type | event_code | param_len | params... |
| 0x04 |    0x0E    |    XX     |   ....    |  <- Command Complete
```

The opcode is little-endian 16-bit; for example `LE_Transmitter_Test` is `0x201E`, sent as `1E 20`.

### 11.5 Switching back to the application state

The SDK provides a software inverse operation, `uart_ipc_path_revert()` (implementation in `bluetooth_config.c:1634`), which restores the state without a reset:

```c
extern void uart_ipc_path_revert(void);

uart_ipc_path_revert();
/* Afterwards:
   - rx_indicate / open_flag are restored to their pre-switch values
   - log_pause(false) restores ulog output
   - trans_en=0 tells the fwd2mb / fwd2uart forwarding threads to stop moving data
   - the LCPU low-power activity lock is released
*/
```

**Note the limitations of the software switch-back**:

- `uart_ipc_path_revert` does not destroy the `f2mb_th` / `f2uart_th` forwarding threads; they remain alive and simply stop moving data because `trans_en=0`
- It does not call `rt_device_close`, so the UART device's ref_cnt is not restored
- If later application code re-takes the UART (`rt_device_open` / setting a new `rx_indicate`), make sure its re-entry logic does not rely on "the device having never been opened before"

If the production flow requires "returning fully to the application state after RF testing, leaving no SDK background threads", a reset path is still recommended; `uart_ipc_path_revert` is suited to repeated switching during debugging/development, avoiding a hardware restart each time.

---

## 12. Verifying Your Implementation

Verify in the following order; if any step fails, do not proceed.

### 12.1 The command itself works

Send the command, then check the console for the line printed by the `BTS2MU_GAP_ENB_DUT_MODE_CFM` handler (using the BT signaling DUT as an example):

```
D/btapp_ge:  << enb DUT mode : <step_num>
```

Implementation: [bts2_app_generic.c:1738-1752](D:/OpenSiFli/SiFli-SDK/middleware/bluetooth/service/bt/bt_finsh/bts2_app_generic.c#L1738-L1752). `<step_num>` is a step counter; the exact number depends on the implementation and should not be checked as a fixed value. **The criterion is: this log appears, and is not accompanied by `<< failed to enb DUT mode!`** (the latter is triggered by `res != BTS2_SUCC`).

**If you see neither line**, the command did not reach the LCPU; trace back the `BLE_POWER_ON_IND` timing, whether the protocol stack started, whether the handler actually executed, and whether `CONFIG_BT_FINSH=y` is enabled (otherwise this trace does not exist).



### 12.2 Confirming the scan mode state

`gap_enb_dut_mode_req` does not enable scan automatically; whether the instrument can find the DUT depends on whether the host previously called `bt_interface_set_scan_mode(1, 1)` or `gap_wr_scan_enb_req(tid, 1, 1)`. Read the current cached value to confirm:

```c
uint8_t scan = bt_interface_get_current_scan_mode();
LOG_I("scan mode = %d", scan);
/* 0 = none; 1 = inquiry only; 2 = page only; 3 = both inquiry and page */
```

If the host set scan_mode=3 before issuing the DUT command, you expect to read 3 here; otherwise it returns the actual scan state. Reading anything other than 3 means your initialization flow did not enable scan — add `bt_interface_set_scan_mode(1, 1)` before issuing the DUT command.

### 12.3 HCI pass-through link verification (Template B)

After switching the UART, use a PC serial tool (in hex mode) to send HCI Reset:

```
TX: 01 03 0C 00
RX: 04 0E 04 XX 03 0C 00
```

Receiving a response means the HCI pass-through link is working. `XX` is the num_hci_cmd_packets field and can be any value; what matters is the leading `04 0E` (Command Complete event) and the trailing `03 0C 00` (the associated HCI_Reset opcode + status 0).

Then send LE Transmitter Test:

```
TX: 01 1E 20 03 13 25 00      LE_Transmitter_Test ch19, 37B, PRBS9
RX: 04 0E 04 XX 1E 20 00       Command Complete, status 0
```

The controller starts transmitting continuously on ch19; at this point a spectrum analyzer or the CMW500 can see a signal at 2440 MHz.

### 12.4 Full RF metric verification

Connect a CMW500 (or an equivalent such as the R&S CBT or Anritsu MT8852B), run the test suite per the SIG DTM standard, and check whether TX power, frequency offset, and PER are within spec. This step is done on the instrument and requires no code from you.

---

## 13. Complete Integration Flow Example

Assume you already have an RT-Thread + SiFli SDK project running its own command system (this section is general, not limited to finsh / a custom protocol). Below are the steps to fully integrate a "DUT test command", using **Template A (signaling DUT, UART not switched)** as the example.

### Step 1: Add configuration to proj.conf

```
CONFIG_BT_FINSH=y
CONFIG_RT_USING_FINSH=y
# Template A does not need CONFIG_BT_RF_TEST=y; only Template B does
```

### Step 2: Add a handler in your command-handling code

```c
#include "bts2_app_inc.h"     /* contains gap_api.h */

static int s_dut_armed = 0;

static void cmd_dut_handler(...)
{
    if (!app_stack_ready())
    {
        respond_error("stack not ready");
        return;
    }

    if (s_dut_armed)
    {
        respond_error("already in DUT");
        return;
    }

    gap_enb_dut_mode_req(bts2_task_get_app_task_id());
    s_dut_armed = 1;
    respond_ok();
}
```

The `s_dut_armed` flag prevents repeated calls from confusing the LCPU state.

### Step 3: Hook into command dispatch

According to your project's command system:
- finsh: `MSH_CMD_EXPORT(cmd_dut_handler, enter BT signaling DUT mode)`
- command-table dispatch: add a row `{ "DUT", cmd_dut_handler }`
- BLE/SPP remote trigger: call it when a specific opcode is received

### Step 4: Rebuild, reflash, and power on

Confirm the power-on flow has completed and that an application command (any existing command) responds normally, proving the protocol stack and command system are ready.

### Step 5: Send the DUT command

After receiving the `OK` response, immediately check the console log; you expect to see `<< enb DUT mode : <step_num>` as described in §12.1, without `<< failed to enb DUT mode!`.

### Step 6: Connect the instrument and run the test

Configure the CMW500's Bluetooth Signaling test module, set DUT Connection to BD Address (read it out with `Read_BD_Addr` and fill it in), and start the test suite. The instrument automatically runs inquiry -> page -> ACL -> LMP_test_control, executing the full BQB test matrix.

### Step 7: Exit the test

Simply reset the chip.

---

If you are doing Template B (HCI pass-through), Step 1 must also add `CONFIG_BT_RF_TEST=y` and the IPC UART configuration (see Chapter 1); the Step 2 handler body is replaced with the Template B code plus an implementation of `my_app_release_uart()`; after Step 5, use the §12.3 PC script for verification instead; in Step 6, connect the instrument directly to the switched-away UART (no BT Address flow needed — the instrument sends HCI commands directly).

---

## 14. Troubleshooting

| Symptom | Cause | Solution |
|---|---|---|
| `gap_enb_dut_mode_req` does nothing | The protocol stack hasn't started | Call it after `BLE_POWER_ON_IND` |
| The `<< enb DUT mode` log doesn't appear | The LCPU didn't receive the command | Check whether mailbox / sifli_msg_send works |
| After `bt_rftest enter`, `bttx` reports "please enter test mode first" | The state machine was disrupted | Reset and run through the flow again |
| No HCI response after `bt_cm uart_dut` | `CONFIG_BT_RF_TEST=y` is not enabled, so this command branch does not call `uart_ipc_path_change` | Check `.config`, confirm `BT_RF_TEST=y`; or bypass finsh and call the `uart_ipc_path_change()` API directly |
| After `uart_ipc_path_change`, the controller receives no commands | The UART is still held by the original protocol-stack thread | Close the device and unbind rx_indicate before switching |
| After switching the UART, the RX thread asserts `dev != NULL` | The calling thread returned to the main loop and accessed the device again | The caller must permanently suspend with `rt_thread_suspend(rt_thread_self())` |
| HCI commands occasionally drop bytes | 1 Mbps is unstable on some USB-UART adapters | Drop to 921600 or 460800 |
| packet_count reads 0 after a BLE TX test | This is by spec | Only RX tests have a packet_count |
| TX power is not as expected in a BT non-signaling test | `pwr_lvl` is a dBm integer (typically 0..13), not a level index | Enter the target dBm value directly; the controller quantizes it at runtime to a hardware level supported by the PA (see §7.1); default to 10 |

---

## 15. Key Source Locations

| Module | File |
|---|---|
| BT signaling DUT API | `gap_api.h` (declaration); implemented in the BT protocol-stack lib (.a) |
| BT non-signaling / BLE DTM API | `bf0_ble_common.h` / `bf0_bt_common.c` |
| `bt_cm dut` / `bt_cm uart_dut` implementation | `service/bt/bt_cm/bt_connection_manager.c` lines 1750-1778 |
| `bt_rftest` implementation | `service/common/bf0_bt_common.c` lines 979-1210 |
| `uart_ipc_path_change` implementation | `service/internal/bluetooth_config.c` line 1539+ |
| HCI pass-through threads / IPC mailbox | same file as above, `hci_forward_to_mb_entry` / `hci_forward_to_uart_entry` |

---

## 16. Reference Examples and Documents

- Base example for BT signaling / non-signaling / individual / DTM testing: any BT example under [`example/bt/`](../../../../example/bt/), e.g. [`example/bt/spp/`](../../../../example/bt/spp/)
- HCI-pass-through-at-boot example (driven directly by a tester / PC script): [`example/bt/HCI_over_uart/`](../../../../example/bt/HCI_over_uart/)
- The SiFli_RfTool tool itself: `tools/SiFli_RfTool/SiFli_RfTool.exe` (shipped with the SDK, no installation required)
- SiFli_RfTool user manual: [SiFli_RfTool_UM](../tools/SiFli_RfTool/SiFli_RfTool_UM.md)
- Individual test and tester operation guide: [SiFli SF32LB5xx Bluetooth Individual RF Test Guide](bt_individual_test_guide.md)
