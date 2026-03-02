# type (r[1-9];w;u),circuit,name,[comment],[QQ],ZZ,PBSB,[ID],field1,part (m/s),datatypes/templates,divider/values,unit,comment
#,BAI00,eloBLOCK,18/14 kW (PROD=0010023658 SW0109 HW7503),,,,,,,,,,
*r,,,,,,B509,0D,,,,,,
*w,,,,,,B509,0E,,,,,,
*wi,#install,,,,,B509,0E,,,,,,
*ws,#service,,,,,B509,0E,,,,,,
*[SW],scan,,,SW,,,,,,,,,

# ##### dia level 1 #####,,,,,,,,,,,,,
# StatusList returns ERR: invalid position on eloBLOCK HW7503 — register not available
#r,,StatusList,StatusList_DK,,,,B509,,,HEX:6,,,Statustexte ergänzen
r;wi,,PartloadHcKW,d.00 heating partload,,,,6C00,,,power,,,Heating part load
r;wi,,WPPostrunTime,d.01 central heating overruntime,,,,6400,,,minutes0,,,water pump overrun time for heating mode
r1,,StorageTemp,d.04 Storage temperature,,,,1700,,,tempsensor,,,Current temperature for warm start sensor (combi boiler) / Current storage tank sensor (system boiler only)
r,,FlowTempDesired,d.05 flow/return setpoint,,,,3900,,,temp,,,Flow temperature target value or return target value when return regulation is set
r,,StorageTempDesired,d.07 Storage setpoint,,,,0400,,,temp,,,"Warm start temperature value (combination boiler plus only), Storage temperature target value (system boiler only)"
r,,WP,d.10 Central heating pump,,,,4400,,,onoff,,,internal heating pump
r,,extWP,d.11 external pump,,,,3F00,,,onoff,,,External heating pump
r,,CirPump,d.13 Circulation pump,,,,7B00,,,onoff,,,Hot water circulation pump (via external module)
r;wi,,PumpPowerDesired,d.14 Desired heating pump power,,,,A100,,,UCH,0=auto;1=53;2=60;3=70;4=85;5=100,%,PWM-Desired central heating pump power,,,
r,,WPPWMPower,d.15 WP_PWMPower_DK,,,,"7300",,,UCH,,,actual PWM-Powerrate of electronic-pump
r1,,DCRoomthermostat,d.16 room thermostat 24 V,,,,0E00,,,onoff,,,External controls heat demand (Clamp 3'-4')
r,,HwcDemand,d.22 DHW demand,,,,5800,,,yesno,,,domestic hot water (tapping or tank contact) demand
r,,HeatingDemand,d.23 Heating demand,,,,4000,,,yesno,,,Heating demand
r,,StoragereleaseClock,d.25 DHW demand enabled,,,,7100,,,yesno,,,hot water release (tank storage) via eBus Control
r,,PrimaryCircuitFlowRate,d.29 PrimaryCircuitFlowRate_DK,,,,FB00,,,uin100,,,primary circuit water flow rate
r1,,PositionValveSet,d.35 Position TWV,,,,5400,,,UCH,,,"Position of diverter valve; 100 = DHW, 0 = heating, 40 = middle position"
r1,,FlowTemp,d.40 TFT_DK,,,,1800,,,tempsensor,,,flow temperature
r,,OutdoorstempSensor,d.47 Temp outside,,,,7600,,,tempsensor,,,Outside temperature (uncorrected value)

# ##### dia level 2 #####,,,,,,,,,,,,,
r;wi,,HcPumpMode,d.18 CH pump mode,,,,B700,,,UCH,0=post run;1=permanent;2=winter,,Pump mode
r;wi,,SecondPumpMode,d.19 Mode of 2 speed pump,,,,A000,,,UCH,,,"Operating mode of the two-stage heating pump; 0: burner mode=2/pre/post-run=1; 1: CH+post-run=1/DHW=2; 2: as 1 but speed in CH depends on partload (default); 3: always stage 2"
r;wi,,HwcTempMax,d.20 Max value tanksetpoint,,,,D900,,,temp,,,Adjustment of maximum tank setpoint range (maximum endposition of potmeter)
r,,DeactivationsTemplimiter,d.60 Number STL cut off,,,,2000,,,UCH,,,Number of safety temperature limiter cut offs
r;wi,,FlowsetHcMax,d.71 Max value flow temp,,,,A500,,,temp,,,Adjustment of maximum central heating setpoint range (maximum endposition of potmeter)
# HwcPostrunTime: DHW storage-specific, returns ERR: invalid position on eloBLOCK
#r;wi,,HwcPostrunTime,d.72 postrun time storage,,,,1104,,,UCH,-10,s,Pump overrun time
# StorageLoadTimeMax: DHW storage-specific, returns ERR: invalid position on eloBLOCK
#r;wi,,StorageLoadTimeMax,d.75 Max load time storage,,,,1204,,,minutes0,,,Maximum charging time
r,,CodingResistor,d.76 SD_CodingResistor_DK,,,,"9200",,,HEX:3,,,boiler identification resistor
# PartloadHwcKW / FlowsetHwcMax: DHW storage-specific, returns ERR: invalid position on eloBLOCK
#r;wi,,PartloadHwcKW,d.77 hot water partload,,,,0804,,,power,,,storage part load
#r,,FlowsetHwcMax,d.78 Max value flow temp storage,,,,0F04,,,temp,,,storage charging temperature limit
r,,HcHours,d.80 operating hours CH,,,,2800,,,hoursum2,,,central heating operation hours
r,,HwcHours,d.81 Operating hours DHW,,,,2200,,,hoursum2,,,water heating operation hours
r,,HcStarts,d.82 Burner starts heating,,,,2900,,,UIN,-100,,cycles in heating mode
r,,HwcStarts,d.83 Burner starts DHW,,,,2300,,,UIN,-100,,Cycles in hot water operation
r;wi,,HoursTillService,d.84 Hours till service,,,,"AC00",,,hoursum2,,,Hours left before service is needed,,,
# EBusHeatcontrol: returns ERR: invalid position on eloBLOCK HW7503
#r,,EBusHeatcontrol,d.90 Digital control recognized,,,,0004,,,yesno,,,Digital regulator status
r,,dcfState,d.91 Status DCF 77,,,,"6900",,,dcfstate,,,DCF status
r;ws,,DSNOffset,d.93 Device specific number,,,,3004,,,UCH,,,DSN appliance variant setting
r;ws,,SetFactoryValues,d.96 Set factory settings,,,,6804,,,yesno,,,Factory settings

# ##### maintenance data #####,,,,,,,,,,,,,
r,,TempGradientFailure,TempGradientFailure_DK,,,,1100,,,temp0,,,Number of cut offs of the boilers cause of to high gradient (S.54)
r,,TempDiffBlock,TempDiffBlock_DK,,,,1200,,,temp0,,,Number of modulationblocking of the boilers cause of to high/incorrect difference of flow/return temperatures
r,,TempDiffFailure,TempDiffFailure_DK,,,,1300,,,temp0,,,Number of cut offs of the boilers cause of to high/incorrect differences of flow/return temperatures
r,,PumpHours,Pump operation hours,,,,1400,,,hoursum2,,,pump operation hours
r,,HcPumpStarts,CH_PumpCommunt_DK,,,,1500,,,cntstarts2,,,pump switchings
r,,ValveStarts,TWV_Communt_DK,,,,1A00,,,cntstarts2,,,commutations of the three way valve
r,,OverflowCounter,OverflowCounter_DK,,,,1E00,,,yesno,,,Predictive Maintenance counter have got an overflow
r,,TempMaxDiffExtTFT,MaxTempDiffExtTFT_DK,,,,2700,,,temp,,,Predictive maintenance data
r,,maintenancedata_HwcTempMax,Temp DHW max,,,,3500,,,temp,,,maximum temperature measured by the domestic hot water outlet sensor
r,,StorageTempMax,Temp storage max,,,,3600,,,temp,,,maximum temperature measured by the external tank sensor
r,,FlowTempMax,TFT_max_DK,,,,3700,,,temp,,,maximum temperature measured by the flow sensor
r,,DeltaFlowReturnMax,MaxDeltaFlowReturn_DK,,,,3D00,,,temp,,,Predictive Maintenance data
r,,StorageLoadPumpHours,TankLoadPumpOperationHours_DK,,,,4C00,,,hoursum2,,,Preditive maintenance data
r,,StorageloadPumpStarts,TankloadPumpCommunt_DK,,,,4F00,,,cntstarts2,,,Preditive maintenance data
r,,HwcWaterflowMax,DHW Waterflow_max_DK,,,,5600,,,uin100,,,domestic hot water flow sensor maximum value
r,,ReturnTempMax,TRT_max_DK,,,,BE00,,,temp,,,max. return temperature
r,,PumpHwcFlowSum,PumpDHWFlowSum_DK,,,,C100,,,UIN,,,summed up DHW flow rate
r,,PumpHwcFlowNumber,PumpDHWFlowNumber_DK,,,,C200,,,UCH,,,number of times DHW flow rate was detected
r,,SHEMaxFlowTemp,SHE_MaxFlowTemp_DK,,,,C300,,,temp,,,maximum flow temperature DHW
r,,SHEMaxDeltaHwcFlow,SHE_MaxDeltaFlowDHW_DK,,,,C400,,,temp,,,maximum difference between flow and DHW outlet temperature
r,,PrEnergySumHwc1,PrEnergySumDHW1_DK,,,,C500,,,ULG,,,Predictive Maintenance data
r,,PrEnergyCountHwc1,PrEnergyCountDHW1_DK,,,,C600,,,ULG,,,Predictive Maintenance data
r,,PrEnergySumHwc2,PrEnergySumDHW2_DK,,,,C700,,,ULG,,,Predictive Maintenance data
r,,PrEnergyCountHwc2,PrEnergyCountDHW2_DK,,,,C800,,,ULG,,,Predictive Maintenance data
r,,PrEnergySumHwc3,PrEnergySumDHW3_DK,,,,C900,,,ULG,,,Predictive Maintenance data
r,,PrEnergyCountHwc3,PrEnergyCountDHW3_DK,,,,CA00,,,ULG,,,Predictive Maintenance data
r,,WaterHcFlowMax,MaxWaterFlowCH_DK,,,,D000,,,UIN,,,Predictive Maintenance data
r,,WaterpressureVariantSum,WaterpressureVariantSum_DK,,,,F000,,,pressm2,,,Predictive Maintenance data
r,,WaterpressureMeasureCounter,WaterpressureMeasureCounter_DK,,,,F100,,,UCH,,,Predictive Maintenance data
r,,PrAPSCounter,PrAPSCounter_DK,,,,F200,,,UCH,,,Predictive Maintenance data
r,,PrAPSSum,PrAPSSum_DK,,,,F300,,,seconds2,,,Predictive Maintenance data
r,,PrVortexFlowSensorValue,PrVortexFlowSensorValue_DK,,,,F400,,,SIN,,ADC,Predictive Maintenance data
r,,PrEnergySumHc1,PrEnergySumCH1_DK,,,,F500,,,ULG,,,Predictive Maintenance data
r,,PrEnergyCountHc1,PrEnergyCountCH1_DK,,,,F600,,,ULG,,,Predictive Maintenance data
r,,PrEnergySumHc2,PrEnergySumCH2_DK,,,,F700,,,ULG,,,Predictive Maintenance data
r,,PrEnergyCountHc2,PrEnergyCountCH2_DK,,,,F800,,,ULG,,,Predictive Maintenance data
r,,PrEnergySumHc3,PrEnergySumCH3_DK,,,,F900,,,ULG,,,Predictive Maintenance data
r,,PrEnergyCountHc3,PrEnergyCountCH3_DK,,,,FA00,,,ULG,,,Predictive Maintenance data

# ##### expert level #####
# externalFlowTempDesired: returns ERR: invalid position on eloBLOCK HW7503
#r,,externalFlowTempDesired,external flowsetpoint,,,,2500,,,temp,,,Flow target temperature from external regulator to terminal 7-8-9
r,,externalHwcSwitch,DHWSwitch_DK,,,,0000,,,onoff,,,Demand external tank (via contact)
r1,,WaterPressure,Waterpressure,,,,0200,,,presssensor,,,water pressure
r,,ChangesDSN,Changes_DSN_DK,,,,0C00,,,UCH,,,Numbers adjusting (storing) the DSN
r,,VolatileLockout,SD_VolatileLockout_DK,,,,1000,,,UCH,240=no;15=yes,,TRUE: STB failure is volatile; FALSE: STB failure is not volatile
r,,HcUnderHundredStarts,HZ_UnderHundred_SwiActi_DK,,,,3000,,,UCH,,,Heat switch cycles under hundred
r,,HwcUnderHundredStarts,BW_UnderHundred_SwiActi_DK,,,,3100,,,UCH,,,DHW switch cycles under hundred
r,,EbusSourceOn,EbusSourceOn_DK,,,,3400,,,onoff,,,Activation signal of the eBus source
r,,ExternalFaultmessage,External_Faultmessage_DK,,,,3E00,,,onoff,,,external fault message
r,,HwcTypes,DHW_Types_DK,,,,4B00,,,UCH,,,DHW type of the appliance
r,,HwcImpellorSwitch,DHW impellor switch,,,,5700,,,yesno,,,domestic hot water (tapping) demand
r,,WarmstartDemand,WarmstartDemand,,,,3A04,,,yesno,,,activation of warmstart
r,,BoilerType,BoilerType_DK,,,,5E00,,,UCH,,,Boiler type of the BMU
r,,ParamToken,ParamToken_DK,,,,6000,,,UCH,,,token for parameter management
r,,expertlevel_ReturnTemp,ReturnTemperature_DK,,,,6B00,,,tempsensor,,,external return sensor
r,,FloorHeatingContact,Floor heating contact,,,,7000,,,onoff,,,Floor heating contact
r,,Templimiter,SD_Temperaturelimiter_DK,,,,7700,,,UCH,240=off;15=on,,feedback signal of the temperature limiter
r,,EbusVoltage,EbusVoltage_DK,,,,7F00,,,onoff,,,feedback signal of the eBus source
r,,Testbyte,Testbyte_DK,,,,9900,,,UCH,,,Testbyte (relevant for the Tester)
r,,DSN,DSN_DK,,,,9A00,,,UIN,,,DSN: Device Specific number
r,,PowerValue,PowerValue,,,,AA00,,,HEX:6,,,"Power Values of the Boiler (min Power, max Power)"
r1,,Statenumber,Statenumber_DK,,,,AB00,,,UCH,,,status number
r,,WaterpressureBranchControlOff,WaterpressureBranchControlOff_DK,,,,AF00,,,onoff,,,water pressure branch control off
r,,DSNStart,DSN start,,,,3104,,,UIN,,,DSN Start address
r,,ExtStorageModulCon,ExtTankModulCon_DK,,,,BF00,,,yesno,,,external tank module connected (VR65)
r,,PartnumberBox,Partnumber_Box,,,,C000,,,HEX:5,,,part number of the eBox
r,,WPSecondStage,WP_SecondStage_DK,,,,ED00,,,onoff,,,Second stage of the pump activated
r,,TemplimiterWithNTC,SD_STL_with_NTC,,,,D200,,,UCH,240=no;15=yes,,Safety temperature limit concept: 1=check with NTC; 0=check with contact
r,,VolatileLockoutIFCGV,SD_VolatileLockout_IFC_GV_DK,,,,D300,,,UCH,240=no;15=yes,,all IFC faults are volatile
# VortexFlowSensor: returns ERR: invalid position on eloBLOCK HW7503 (no vortex flow sensor)
#r,,VortexFlowSensor,VortexFlowSensor_DK,,,,D500,,,UIN,,,"Predictive Maintenance data, CH water flow"
r,,DisplayMode,DisplayMode_DK,,,,DA00,,,UCH,,,Display mode of the appliance
r,,InitialisationEEPROM,InitialisationEEPROM,,,,DC00,,,yesno,,,Initialisation of all EEPROM values (done in production)
r,,TimerInputHc,TimerInputCH_DK,,,,DE00,,,onoff,,,timer input (block heat demand)
r,,DCFTimeDate,DCF_TimeDate_DK,,,,E500,,,HEX:7,,,DCF Time / date stamp
# FlowSetPotmeter: address 03F0 is wrong for eloBLOCK — correct address is F003 but register may not exist on HW7503
#r,,FlowSetPotmeter,FlowSetPotmeter,,,,F003,,,temp,,,flow setpoint potentiometer
# HwcSetPotmeter: DHW-specific, returns ERR: invalid position on eloBLOCK
#r,,HwcSetPotmeter,DHWSetPotmeter,,,,F103,,,temp,,,DHW setpoint potentiometer
r;w,,HeatingSwitch,Heating Switch,,,,F203,,,onoff,,,Winter mode active
r;w,,HwcSwitch,DHWSwitch,,,,F303,,,onoff,,,DHW on/off
r,,ProductionByte,ProductionByte,,,,3E04,,,UCH,,,

# ##### Additional electrical parameters (eloBLOCK specific) #####
# Source: official Vaillant eloBLOCK VE Installation Manual 0020265768_01
# Confirmed D.xxx codes from manual matched to eBUS register addresses

# --- Heating element stages (d.100-d.103, registers are consecutive +1) ---
r;wi,,HeatingStage1,d.100 Stage1 enable,,,,ED01,,,onoff,,,Heating element stage 1 enable/disable
r;wi,,HeatingStage2,d.101 Stage2 enable,,,,EE01,,,onoff,,,Heating element stage 2 enable/disable
r;wi,,HeatingStage3,d.102 Stage3 enable,,,,EF01,,,onoff,,,Heating element stage 3 enable/disable
r,,ActiveStages,d.103 Stages active,,,,F001,,,UCH,,,Active heating stages bitmask (bit0=stage1, bit1=stage2, bit2=stage3)

# --- Power management (d.104-d.108, registers have step +0x11 between addresses) ---
# d.104: MaxPower — configures maximum output power in kW (UCH, 1 byte, 0-28)
r;wi,,MaxPower,d.104 Max power,,,,A201,,,UCH,,kW,Max configured power output

# d.105: TotalEnergy — total energy consumed since reset (ULG, 4 bytes, kWh)
# Use for Home Assistant Energy Dashboard (state_class: total_increasing)
r,,TotalEnergy,d.105 Energy total,,,,B301,,,ULG,,kWh,Total energy consumption (kWh)

# d.106: ElementHours — total operating time of heating elements.
# Type: hoursum2 — by analogy with gas boiler HcHours (2800), not independently verified on eloBLOCK.
# ⚠️ NOT VERIFIED in any public source for 0010023658.
r,,ElementHours,d.106 Elements runtime,,,,C401,,,hoursum2,,,Heating elements total operating hours (type unverified)

# d.107: OverTempStatus — overtemperature protection trigger value (temp type)
r,,OverTempStatus,d.107 Overtemp status,,,,D501,,,temp,,,Over-temperature protection status

# d.108: CurrentPower — real-time power output.
# Note: Official manual D.155 also describes "current output (continuously updated)".
# D.108 (E601) may be the eBUS-readable value; D.155 may be display-only.
# Address E601 confirmed present in bus grab.
# Type: HEX:2 — applied based on @Akeebaby (GitHub ebusd-configuration issue #254, model 0010023684 HW7503 SW0108).
# ⚠️ NOT VERIFIED on 0010023658 SW0109. @Akeebaby also commented this out as "total error" on their unit.
# Alternative: try UCH (1 byte) if HEX:2 returns ERR.
r,,CurrentPower,d.108 Power now,,,,E601,,,HEX:2,,kW,Current real-time power output (unverified type; source: @Akeebaby issue #254)

# --- Power limiter via dry contact (from official manual D.149, D.152-D.155) ---
# IMPORTANT: D.152 and D.153 allow SOFTWARE power limiting without dry contact hardware!
# eBUS register addresses searched across ALL public sources (GitHub issues/PRs/discussions,
# HA Community, OpenEnergyMonitor, ioBroker, Reddit, ManualsLib, ab-log.ru, 4PDA) — NOT FOUND.
# Method to find: use 'ebusctl grab result all' while changing D.152/D.153 on boiler display.
#
# D.149 — Pump fault detail (fault F.075 explanation):
#   0=no fault, 1=pump blocked, 2=electrical pump fault, 3=dry fire,
#   4=voltage at pump too low, 5=pressure sensor fault, 6=no PWM signal from pump
# r,,PumpFaultDetail,d.149 Pump fault F.75,,,,????01,,,UCH,0=ok;1=blocked;2=elec fault;3=dry fire;4=low voltage;5=press sensor;6=no PWM,,Pump fault detail for F.075
#
# D.152 — Output limiter phase selection (writable!):
#   0=no limit, 1=phase1, 2=phase2, 3=phase3, 4=all phases
#   Allows SOFTWARE power limiting without physical dry contact!
# r;wi,,PowerLimiterPhase,d.152 Output limiter phase,,,,????01,,,UCH,0=no limit;1=phase1;2=phase2;3=phase3;4=all phases,,Phase for power output limiting
#
# D.153 — Output limiter value in kW (writable!):
#   Subtracted from current output in configured phase(s).
#   Example: MaxPower=14kW, D.152=4(all phases), D.153=6kW → output=8kW
# r;wi,,PowerLimiterValue,d.153 Output limiter value,,,,????01,,,UCH,,kW,Power limiting value in kW (subtracted from max output)
#
# D.154 — Frost protection deactivation (writable)
# r;wi,,FrostProtection,d.154 Frost protection,,,,????01,,,onoff,,,Frost protection function active
#
# D.155 — Current power display (real-time, read-only):
#   "The product's current output (continuously updated information)"
#   May be the same as d.108/E601 or a separate display-only register.
# r,,CurrentPowerDisplay,d.155 Current power,,,,????01,,,UCH,,kW,Current output (continuously updated)

!include,errors.inc
