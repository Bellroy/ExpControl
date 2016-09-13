#include <utility.h>
#include <formatio.h>
#include <userint.h>
#include "tools.h"
//#include "nidaqcns.h"  
#include "CONTROL_LoadSave.h"
#include "CONTROL_CalculateOutput.h"
#include "CONTROL_GUI_ccd.h"
#include "CONTROL_GUI_ExtDev.h"



#define WFM_FROM_FILE_MAX_VALUE 1.0
#define WFM_FROM_FILE_MIN_VALUE -1.0


int saveSequenceFile_OldPrgVersion (const char *filename, t_sequence *seq);



/************************************************************************/
/*
/*    Definitions of data elements in for *.seq files
/* 
/*	  (sections and tags for functions Ini_... (see Libaray inifile.c)
/*
/************************************************************************/

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//
// 	WARNING: DO UNTER NO CIRCUMSTANCES CHANGE THE NAMES OF
// 			 THESE STRINGS!!!
//
// 			 THIS WILL AFFECT THE COMPATIBILITY TO 
//			 PREVIOUS PROGRAM VERSIONS.
//
//           Any modification should only ADD new variables or
//           sections, which are not recognized by older program versions.
//
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

const char CONF_TAG_LAUNCHED[] = "launched";

// general file info  
const char INI_SECTION_FILEINFO[]					= "FileInfo";
const char INI_TAG_FILEINFO_filename[]				= "filename";
const char INI_TAG_FILEINFO_Version[]				= "ProgramVersion";
const char INI_TAG_FILEINFO_startNr[]				= "startNr";

// general information for the sequence
const char INI_SECTION_SEQUENCEINFO[]				= "SequenceInfo";
const char INI_TAG_SEQUENCEINFO_description[]   	= "description";
const char INI_TAG_SEQUENCEINFO_nRuns[]   	        = "nRuns";
const char INI_TAG_SEQUENCEINFO_nRepetitions[] 	    = "nRepetitions";
const char INI_TAG_SEQUENCEINFO_nCopies[]   	    = "nCopies";
const char INI_TAG_SEQUENCEINFO_nCounters[]   	    = "nCounters";
const char INI_TAG_SEQUENCEINFO_redoRepetitions[]   = "redoRepetitions";
const char INI_TAG_SEQUENCEINFO_redoRuns[]   	    = "redoRuns";
const char INI_TAG_SEQUENCEINFO_considerWatchdog[]  = "considerWatchdog";
const char INI_TAG_SEQUENCEINFO_generateRandomCounterData[] = "generateRandomCounterData";
const char INI_TAG_SEQUENCEINFO_seqShortName[] = "seqShortName";

const char INI_TAG_SEQUENCEINFO_nDigitalBlocks[]    = "nDigitalBlocks";
const char INI_TAG_SEQUENCEINFO_nWaveforms[]   		= "nWaveforms";
const char INI_TAG_SEQUENCEINFO_wfmTableSortMode[]  = "wfmTableSortMode";
const char INI_TAG_SEQUENCEINFO_nExternalDevices[]  = "nExternalDevices";
const char INI_TAG_SEQUENCEINFO_extdevTableSortMode[]  = "extdevTableSortMode";
const char INI_TAG_SEQUENCEINFO_nTransferFunctions[] = "nTransferFunctions";
const char INI_TAG_SEQUENCEINFO_nGpibCommands[]   	= "nGpibCommands";
const char INI_TAG_SEQUENCEINFO_nCCDSettings[]   	= "nCCDSettings";

// idle seq and auto focus
const char INI_TAG_SEQUENCEINFO_enableFocusFeedback[] = "enableFocusFeedback";	
const char INI_TAG_SEQUENCEINFO_manualPifocFocusVoltage[] = "manualPifocFocusVoltage";

const char INI_TAG_SEQUENCEINFO_autoFocusNumImages[] = "autoFocusNumImages";
const char INI_TAG_SEQUENCEINFO_autoFocusCenterVoltage[] = "autoFocusCenterVoltage";
const char INI_TAG_SEQUENCEINFO_autoFocusStepVoltage[] = "autoFocusStepVoltage";
const char INI_TAG_SEQUENCEINFO_autoFocusFirstBlock[] = "autoFocusFirstBlock";
const char INI_TAG_SEQUENCEINFO_isOfTypeIdleSeq[] = "isOfTypeIdleSeq";  

											   
// analog-output
const char INI_SECTION_DACSETUP[] 					= "DAC_SETUP";
const char INI_TAG_DACSETUP_Timebase_50ns[]   		= "Timebase_50ns";
const char INI_TAG_DACSETUP_Timebase[] 		  		= "Timebase";
const char INI_TAG_DACSETUP_PatternInterval[] 		= "PatternInterval";
const char INI_TAG_DACSETUP_OutputPorts[] 	  		= "OutputPorts";
const char INI_TAG_DACSETUP_nChannels[]		  		= "nChannels";
const char INI_TAG_DACSETUP_Trigger[]	  	  		= "Trigger";
const char INI_TAG_DACSETUP_triggerPolarity[]	  	= "triggerPolarity";
const char INI_TAG_DACSETUP_Name[] 					= "Name%s";
const char INI_TAG_DACSETUP_tableRowHeight[] 		= "tableRowHeight";
const char INI_TAG_DACSETUP_tableHeight[]      		= "tableHeight";

//const char INI_TAG_DACSETUP_voltageSweepsStart[]    = "voltageSweepsStart";
//const char INI_TAG_DACSETUP_voltageSweepsStop[]     = "voltageSweepsStop";
//const char INI_TAG_DACSETUP_voltageSweepsRep[]      = "voltageSweepsRep";

// analog blocks
const char INI_SECTION_ANALOGBLOCK[] 	      		= "AnalogBlock%d";
const char INI_TAG_ANALOGBLOCK_Name[] 		  		= "Name";
const char INI_TAG_ANALOGBLOCK_Waveform[]     		= "Waveform%s";
const char INI_TAG_ANALOGBLOCK_ConstVoltage[] 		= "ConstVoltage%s";

// digital output
const char INI_SECTION_DIOSETUP[] 					= "DIO_SETUP";
const char INI_TAG_DIOSETUP_Name[] 					= "Name%s";
const char INI_TAG_DIOSETUP_ChannelStatus[] 		= "channelStatus";
const char INI_TAG_DIOSETUP_invertDisplay[] 		= "invertDisplay";
const char INI_TAG_DIOSETUP_lockChannel[] 			= "lockChannel";
const char INI_TAG_DIOSETUP_Timebase_50ns[] 		= "Timebase_50ns";
const char INI_TAG_DIOSETUP_Timebase[] 		  		= "Timebase";
const char INI_TAG_DIOSETUP_PatternInterval[] 		= "PatternInterval";
const char INI_TAG_DIOSETUP_OutputPorts[] 	  		= "OutputPorts";
const char INI_TAG_DIOSETUP_nChannels[]		  		= "nChannels";

// digital blocks
const char INI_SECTION_DIGITALBLOCK[]				= "DigitalBlock%d";
const char INI_TAG_DIGITALBLOCK_Name[]  			= "Name";
const char INI_TAG_DIGITALBLOCK_groupName[]  		= "groupName";
const char INI_TAG_DIGITALBLOCK_groupNr[]			= "group";
const char INI_TAG_DIGITALBLOCK_waitForExtTrigger[]	= "waitForExtTrigger";
const char INI_TAG_DIGITALBLOCK_disable[]			= "disable";

const char INI_TAG_DIGITALBLOCK_isGroupExpanded[]   = "isGroupExpanded";
const char INI_TAG_DIGITALBLOCK_gpibCommandNr[]		= "gpibCommandNr";
const char INI_TAG_DIGITALBLOCK_Time[]				= "Time";
const char INI_TAG_DIGITALBLOCK_AnalogBlock[] 		= "AnalogBlock";
const char INI_OLD_TAG_DIGITALBLOCK_AnalogBlock[] 	= "AnalogSequence";
const char INI_TAG_DIGITALBLOCK_BlockMode[] 		= "BlockMode";
const char INI_TAG_DIGITALBLOCK_VariableTime[] 		= "VariableTime";
const char INI_TAG_DIGITALBLOCK_IncrementTime[] 	= "IncrementTime";
const char INI_TAG_DIGITALBLOCK_VariableTimeRep[]  	= "VariableTimeRep";
const char INI_TAG_DIGITALBLOCK_VariableTimeRepOffset[]  	= "VariableTimeRepOffset";
const char INI_TAG_DIGITALBLOCK_VariableTimeStep[] 	= "VariableTimeStep";
const char INI_TAG_DIGITALBLOCK_VariableTimeModulo[] = "VariableTimeModulo";
const char INI_OLD_TAG_DIGITALBLOCK_Ch[] 				= "Ch%s";
const char INI_TAG_DIGITALBLOCK_Channels[] 			= "Channels";
const char INI_TAG_DIGITALBLOCK_alternate[]			= "alternate";
const char INI_TAG_DIGITALBLOCK_isAbsTimeReference[]= "isAbsoluteTimeReference";
const char INI_TAG_DIGITALBLOCK_constVoltages[]	    = "constVoltages";
const char INI_TAG_DIGITALBLOCK_waveforms[]		    = "waveforms";
const char INI_TAG_DIGITALBLOCK_varyPulse[]         = "varyPulse";
const char INI_TAG_DIGITALBLOCK_varyPulseStartIncr[] = "varyPulseStartIncr";
const char INI_TAG_DIGITALBLOCK_varyPulseStopIncr[] = "varyPulseStopIncr";
const char INI_TAG_DIGITALBLOCK_varyPulseStepRep[] = "varyPulseStepRep";

const char INI_TAG_DIGITALBLOCK_doTriggerLoop[] = "doTriggerLoop";
const char INI_TAG_DIGITALBLOCK_doRecvTriggerLoop[] = "doRecvTriggerLoop";

// waveforms
const char INI_SECTION_WFM[] 		= "Waveform%d";
const char INI_TAG_WFM_Name[] 	  	= "Name";
const char INI_TAG_WFM_channel[] 	= "channel";
const char INI_TAG_WFM_hide[] 	  	= "hide";
const char INI_TAG_WFM_Filename[] 	= "Filename";
const char INI_TAG_WFM_uScaleFrom[] = "uScale";
const char INI_TAG_WFM_uScaleTo[]   = "uScaleTo";
const char INI_TAG_WFM_uScaleVary[] = "uScaleVary";
const char INI_TAG_WFM_offsetFrom[] = "offset";
const char INI_TAG_WFM_offsetTo[]   = "offsetTo";
const char INI_TAG_WFM_offsetVary[] = "offsetVary";
const char INI_TAG_WFM_uList[] 		= "uList";
const char INI_TAG_WFM_iList[] 		= "iList";
const char INI_TAG_WFM_tScaleFrom[] = "tScale";
const char INI_TAG_WFM_tScaleTo[] 	= "tScaleTo";
const char INI_TAG_WFM_tScaleVary[] = "tScaleVary";
const char INI_TAG_WFM_timebaseStretch[] = "timebaseStretch";
// external devices
const char INI_SECTION_EXTDEV[] 		= "ExternalDevice%d";
const char INI_TAG_EXTDEV_Name[] 	  	= "Name";
const char INI_TAG_EXTDEV_address[] 	= "Addresse";
const char INI_TAG_EXTDEV_port[] 	  	= "Port";
const char INI_TAG_EXTDEV_type[] 		= "Type";
const char INI_TAG_EXTDEV_uniqueID	[] 	= "UniqueID";

const char INI_TAG_WFM_NumberOfPoints[] = "NumberOfPoints";

const char INI_TAG_WFM_stepFrom[] 	= "stepFrom";
const char INI_TAG_WFM_stepTo[] 	= "stepTo";
const char INI_TAG_WFM_stepRepetition[] = "stepRepetition";
const char INI_TAG_WFM_stepChangeMode[]	= "stepChangeMode";
const char INI_TAG_WFM_stepAlternate[] = "stepAlternate";

const char INI_TAG_WFM_functionID[] = "functionID";
const char INI_TAG_WFM_functionParametersFrom[] = "functionParametersFrom";
const char INI_TAG_WFM_functionParametersTo[]   = "functionParametersTo";
const char INI_TAG_WFM_functionParametersVary[] = "functionParametersVary";
const char INI_TAG_WFM_functionParametersList[] = "functionParametersList%d";
const char INI_TAG_WFM_functionReverseTime[] = "functionReverseTime";


const char INI_TAG_WFM_addrLinkWfm[] 	 = "addrLinkWfm";
const char INI_TAG_WFM_addrType[]		 = "addrType";
const char INI_TAG_WFM_addrMatrix[]		 = "addrMatrix";
const char INI_TAG_WFM_stepAddrMatrix[]	 = "stepAddrMatrix";  
const char INI_TAG_WFM_addDisorder[]	 = "addDisorderAddressing";  
const char INI_TAG_WFM_disorderSeed[]	 = "disorderSeedAddressing";
const char INI_TAG_WFM_disorderTyp[]	 = "disorderTypAddressing";

const char INI_TAG_WFM_addrMatricesPerRepetition[] = "addrMatricesPerRepetition";  
const char INI_TAG_WFM_addrCalibration[] = "addrCalibration";
const char INI_TAG_WFM_addrCalibrationStep[] = "addrCalibrationStep";
const char INI_TAG_WFM_addrTrajParams[] = "addrTrajParams";
const char INI_TAG_WFM_addrCalibrationStepTo[] = "addrCalibrationStepTo";
const char INI_TAG_WFM_addrCenter[] 	 = "addrCenter";
const char INI_TAG_WFM_addrTestFilename[] = "addrTestFilename";
const char INI_TAG_WFM_addrTestFilename2[] = "addrTestFilename2";
const char INI_TAG_WFM_addrEnablePhaseFeedback[] = "addrEnablePhaseFeedback";
const char INI_TAG_WFM_addrEnablePhaseFeedback2[] = "addrEnablePhaseFeedback2";
const char INI_TAG_WFM_addrCamera[] = "addrCamera";
const char INI_TAG_WFM_addrImageNo[] = "addrImageNo";
const char INI_TAG_WFM_addrAddLastOffset[] = "addrAddLastOffset";
const char INI_TAG_WFM_addrAddLastOffsetNotAtFirst[] = "addrAddLastOffsetNotAtFirst";
const char INI_TAG_WFM_addrImageSize[] = "addrImageSize";

const char INI_TAG_WFM_DMD_shapeID[] = "DMDshapeID";
const char INI_TAG_WFM_DMD_shapeParametersFrom[] = "DMDshapeParametersFrom";
const char INI_TAG_WFM_DMD_shapeParametersTo[] = "DMDshapeParametersTo";
const char INI_TAG_WFM_DMD_shapeParametersVary[] = "DMDshapeParametersVary";    
const char INI_TAG_WFM_DMD_bitmapSize[] = "DMDbitmapSize";   
const char INI_TAG_WFM_DMD_enable[] = "DMDenable";     
const char INI_TAG_WFM_DMD_enableDMDPhaseFeedback[] = "DMDPhaseFeedback"; 
const char INI_TAG_WFM_DMD_enableDMDErrorDiffusion[] = "DMDErrorDiffusion";
const char INI_TAG_WFM_DMD_enableDMDSmoothing[] = "DMDSmoothing"; 

const char INI_TAG_WFM_add[] 		= "add";
const char INI_TAG_WFM_type[]		= "wfmType";
const char INI_TAG_WFM_Voltage[]    = "Voltage%d";  
const char INI_TAG_WFM_V[]      	= "V%d";
const char INI_TAG_WFM_Interval[]   = "Interval%d";
const char INI_TAG_WFM_transferFunction[]   = "transferFunction";
const char INI_TAG_WFM_strValueName [] = "strValueName";
const char INI_TAG_WFM_strValueUnits[] = "strValueUnits";

const char INI_SECTION_TRANSFERFUNCTION[]  = "TransferFunction%d";
const char INI_TAG_TRANSFERFUNCTION_name[] = "name";
const char INI_TAG_TRANSFERFUNCTION_type[] = "type";
const char INI_TAG_TRANSFERFUNCTION_xAxis[] = "xAxis";
const char INI_TAG_TRANSFERFUNCTION_yAxis[] = "yAxis";
const char INI_TAG_TRANSFERFUNCTION_nParameters[] = "nParameters";
const char INI_TAG_TRANSFERFUNCTION_parameters[] = "parameters";
const char INI_TAG_TRANSFERFUNCTION_parameterName[] = "parameterName%d";
const char INI_TAG_TRANSFERFUNCTION_defaultVoltage[] = "defaultVoltage";

const char INI_TAG_POINT_varyTime[]       = "POINT%d_varyTime";
const char INI_TAG_POINT_timeStart[]      = "POINT%d_timeStart";
const char INI_TAG_POINT_timeIncrement[]  = "POINT%d_timeIncrement";
const char INI_TAG_POINT_varyValue[]      = "POINT%d_varyValue";
const char INI_TAG_POINT_valueStart[]     = "POINT%d_valueStart";
const char INI_TAG_POINT_valueIncrement[] = "POINT%d_valueIncrement";
const char INI_TAG_POINT_stepRep[] 		  = "POINT%d_stepRep";

// GPicCommands
const char INI_SECTION_GPIBCOMMAND[] 	      		= "GpibCommand%d";
const char INI_TAG_GPIBCOMMAND_gpibAddress[] 		= "gpibAddress";
const char INI_TAG_GPIBCOMMAND_commandType[]        = "commandType";
const char INI_TAG_GPIBCOMMAND_noGPIBdevice[]       = "noGPIBdevice";
const char INI_TAG_GPIBCOMMAND_extDeviceID[]		= "extDeviceID";
const char INI_TAG_GPIBCOMMAND_extDeviceChannel[]	= "extDeviceChannel";
const char INI_TAG_GPIBCOMMAND_waveform[] 		    = "waveform";
//const char INI_TAG_GPIBCOMMAND_timebase[] 			= "timebase";
//const char INI_TAG_GPIBCOMMAND_timebaseDiv[] 		= "timebaseDiv";
const char INI_TAG_GPIBCOMMAND_channel[] 		    = "channel";
const char INI_TAG_GPIBCOMMAND_nPoints[] 			= "nPoints";
const char INI_TAG_GPIBCOMMAND_duration[] 			= "duration";
const char INI_TAG_GPIBCOMMAND_name[]				= "name";
const char INI_TAG_GPIBCOMMAND_transmitBeforeFirst[]= "transmitBeforeFirstRepetition";
const char INI_TAG_GPIBCOMMAND_transmitBeforeEvery[]= "transmitBeforeEveryRepetition";
const char INI_TAG_GPIBCOMMAND_transmitAfterEvery[]	= "transmitAfterEveryRepetition";
const char INI_TAG_GPIBCOMMAND_transmitAfterLast[]	= "transmitAfterLastRepetition";
const char INI_TAG_GPIBCOMMAND_transmitAfterStartOfSequence[] = "transmitAfterStartOfSequence";
const char INI_TAG_GPIBCOMMAND_maximumDuration[] = "maximumDuration";
																							 
const char INI_TAG_GPIBCOMMAND_askDeviceForErrors[] = "askDeviceForErrors";
const char INI_TAG_GPIBCOMMAND_wfmAlwaysMaxPts[] 	= "wfmAlwaysMaxPts";
const char INI_TAG_GPIBCOMMAND_wfmTransferMode [] 	= "wfmTransferMode";					 

const char INI_TAG_GPIBCOMMAND_enableStep[]		    = "enableStep";
const char INI_TAG_GPIBCOMMAND_startFreq[]		    = "startFreq";
const char INI_TAG_GPIBCOMMAND_stopFreq[]		    = "stopFreq";
const char INI_TAG_GPIBCOMMAND_enterSpan[]			= "enterSpan";
const char INI_TAG_GPIBCOMMAND_addFreqOffset[]		= "addFreqOffset";
const char INI_TAG_GPIBCOMMAND_fetchFreq[]  		= "fetchFreq"; 
const char INI_TAG_GPIBCOMMAND_freqOffsetMHz[]		= "freqOffsetMHz";
const char INI_TAG_GPIBCOMMAND_nStepPoints[]		= "nStepPoints";
const char INI_TAG_GPIBCOMMAND_nStepRepetition[]    = "nStepRepetition";
const char INI_TAG_GPIBCOMMAND_stepTrigger[]		= "stepTrigger";
const char INI_TAG_GPIBCOMMAND_startMultiply[]	    = "startMultiply";
const char INI_TAG_GPIBCOMMAND_stopMultiply[]	    = "stopMultiply";
const char INI_TAG_GPIBCOMMAND_centerMultiply[]	    = "centerMultiply";
const char INI_TAG_GPIBCOMMAND_spanMultiply[]	    = "spanMultiply";
const char INI_TAG_GPIBCOMMAND_divideFreq[]	    	= "divideFreq";
const char INI_TAG_GPIBCOMMAND_commandStringSend[]  = "commandStringSend%d";
const char INI_TAG_GPIBCOMMAND_commandString[]      = "commandString%d";
const char INI_TAG_GPIBCOMMAND_commandStringReceive[] = "commandStringReceive%d";
const char INI_TAG_GPIBCOMMAND_commandStringErrorCheck[] = "commandStringErrorCheck%d";
const char INI_TAG_GPIBCOMMAND_rfOn[]				= "rfOn";
const char INI_TAG_GPIBCOMMAND_outputPower[]		= "outputPower";
const char INI_TAG_GPIBCOMMAND_pulseModeEnable[]	= "pulseModeEnable";
const char INI_TAG_GPIBCOMMAND_pulseModeLogic[]		= "pulseModeLogic";
const char INI_TAG_GPIBCOMMAND_enableFM[]			= "enableFM";
const char INI_TAG_GPIBCOMMAND_freqDeviation[]		= "freqDeviation";
const char INI_TAG_GPIBCOMMAND_freqDeviationMultiply[] = "freqDeviationMultiply";
const char INI_TAG_GPIBCOMMAND_enableAM[] 			= "enableAM";
const char INI_TAG_GPIBCOMMAND_AMdepthPercent[] 	= "AMdepthPercent";

const char INI_TAG_GPIBCOMMAND_enableFeedback[]		= "enableFeedback%d";
const char INI_TAG_GPIBCOMMAND_feedbackAnalogInput[]= "feedbackAnalogInput%d";
const char INI_TAG_GPIBCOMMAND_feedbackFreqDeviation[]="feedbackFreqDeviation%d";
const char INI_TAG_GPIBCOMMAND_feedbackOffset[] 	= "feedbackOffset%d";


const char INI_TAG_GPIBCOMMAND_stepVoltage[]		= "stepVoltage";
const char INI_TAG_GPIBCOMMAND_startVoltage[]		= "startVoltage";
const char INI_TAG_GPIBCOMMAND_stopVoltage[]		= "stopVoltage";
const char INI_TAG_GPIBCOMMAND_stepCurrent[]		= "stepCurrent";
const char INI_TAG_GPIBCOMMAND_startCurrent[]		= "startCurrent";
const char INI_TAG_GPIBCOMMAND_stopCurrent[]		= "stopCurrent";
const char INI_TAG_GPIBCOMMAND_waitUntilStabilized[]= "waitUntilStabilized";

const char INI_TAG_GPIBCOMMAND_startPulseDelay[]    = "startPulseDelay";
const char INI_TAG_GPIBCOMMAND_stopPulseDelay[]     = "stopPulseDelay";   
const char INI_TAG_GPIBCOMMAND_stepPulseDelay[]     = "stepPulseDelay"; 
const char INI_TAG_GPIBCOMMAND_startPulseLength[]   = "startPulseLength"; 
const char INI_TAG_GPIBCOMMAND_stopPulseLength[]    = "stopPulseLength"; 
const char INI_TAG_GPIBCOMMAND_stepPulseLength[]    = "stepPulseLength";


// waveforms
const char INI_SECTION_TABLECONFIG[] = "TableConfig%d";
const char INI_TAG_TABLECONFIG_name[] 	  	= "name";
const char INI_TAG_TABLECONFIG_DIOvisible[] = "DIOvisible";
const char INI_TAG_TABLECONFIG_AOvisible[] 	= "AOvisible";
														 
														 
// for compatibility with versions prior to 1.60
// (which used self-made parser)
const char INI_OLD_SECTION_END[] 			= "[END]";
const char INI_OLD_SECTION_Waveform[] 	  	= "[Waveform]";
const char INI_OLD_SECTION_AnalogBlock[] 	= "[AnalogSequence]";
const char INI_OLD_SECTION_DigitalBlock[] 	= "[DigitalBlock]";
const char INI_OLD_TAG_WFM_V[] 				= "V=";
const char INI_OLD_TAG_WFM_Voltage[]    	= "Voltage=";  
const char INI_OLD_TAG_WFM_Interval[]   	= "Interval=";
const char INI_OLD_TAG_WFM_Filename[]		= "Filename";


const char INI_SECTION_CCDSETTINGS[] = "CCDSettings%d";
const char INI_TAG_CCDSETTINGS_enableCCD[] = "enableCCD";
const char INI_TAG_CCDSETTINGS_serialNo[] 	= "serialNo";
const char INI_TAG_CCDSETTINGS_description[] = "description";     
const char INI_TAG_CCDSETTINGS_cameraType[]  = "cameraType";
const char INI_TAG_CCDSETTINGS_shutter[] = "shutter";
const char INI_TAG_CCDSETTINGS_cooler[] = "cooler";
const char INI_TAG_CCDSETTINGS_fanMode[] = "fanMode";
const char INI_TAG_CCDSETTINGS_temperature[] = "temperature";
const char INI_TAG_CCDSETTINGS_preamplifierGain[] = "preamplifierGain";
const char INI_TAG_CCDSETTINGS_shiftSpeed[] = "shiftSpeed";
const char INI_TAG_CCDSETTINGS_readoutRate[] = "readoutRate";
const char INI_TAG_CCDSETTINGS_acquisition[] = "acquisition";
const char INI_TAG_CCDSETTINGS_picturesPerRepetition[] = "picturesPerRepetition";
const char INI_TAG_CCDSETTINGS_exposureTime_ms[] = "exposureTime_ms";
const char INI_TAG_CCDSETTINGS_takeSubimage[] 	= "takeSubimage";
const char INI_TAG_CCDSETTINGS_enableEMCCD[]	= "enableEMCCD";
const char INI_TAG_CCDSETTINGS_EMCCDgain[]	= "EMCCDgain";
const char INI_TAG_CCDSETTINGS_subimageCenterX[] = "subimageCenterX";
const char INI_TAG_CCDSETTINGS_subimageCenterY[] = "subimageCenterY";
const char INI_TAG_CCDSETTINGS_subimageSizeX[]	= "subimageSizeX";
const char INI_TAG_CCDSETTINGS_subimageSizeY[]  = "subimageSizeY";
const char INI_TAG_CCDSETTINGS_binsize[]	    = "binsize";
const char INI_TAG_CCDSETTINGS_evaluateSubimage[]	= "evaluateSubimage";
const char INI_TAG_CCDSETTINGS_evaluateCenterX[] = "evaluateCenterX";      
const char INI_TAG_CCDSETTINGS_evaluateCenterY[] = "evaluateCenterY";
const char INI_TAG_CCDSETTINGS_evaluateSizeX[] = "evaluateSizeX";
const char INI_TAG_CCDSETTINGS_evaluateSizeY[] = "evaluateSizeY";
const char INI_TAG_CCDSETTINGS_saveRawData[] = "saveRawData";          
const char INI_TAG_CCDSETTINGS_imageProcessing[] = "imageProcessing";       
const char INI_TAG_CCDSETTINGS_dataPath[] = "dataPath";
const char INI_TAG_CCDSETTINGS_date[] = "date";
const char INI_TAG_CCDSETTINGS_time[] = "time";
const char INI_TAG_CCDSETTINGS_daytimeInSeconds[] = "daytimeInSeconds";
const char INI_TAG_CCDSETTINGS_kineticSeriesLength[] = "kineticSeriesLength";
const char INI_TAG_CCDSETTINGS_separateFilePerRepetition[] = "separateFiles";
const char INI_TAG_CCDSETTINGS_nRepetitions[] = "nRepetitions";
const char INI_TAG_CCDSETTINGS_nCycles[] = "nCycles";
const char INI_TAG_CCDSETTINGS_saveData[] = "saveData";
const char INI_TAG_CCDSETTINGS_pixelCount[] = "pixelCount";
const char INI_TAG_CCDSETTINGS_sumPictures[] = "sumPictures";
const char INI_TAG_CCDSETTINGS_fromIdleSequence[] = "fromIdleSequence";
const char INI_TAG_CCDSETTINGS_idleSequenceDataPath[] = "idleSequenceDataPath";
const char INI_TAG_CCDSETTINGS_seqNr[] = "seqNr";  


/************************************************************************/
/*
/*    load waveform from file (*.WFM)
/*    (file contains list of  point values X_i
/* 
/*	  WFM_FROM_FILE_MAX_VALUE >= X_i >= WFM_FROM_FILE_MIN_VALUE  
/*
/************************************************************************/

// pointList is expected to be not initialized (given as pointer to double array)
int WFM_pointlist_from_file(double** pointList,unsigned long* nPoints, const char* filename, double min, double max) {
	int i, fileEnd;
    int file;
    double dval;
    char *komma;
    
    #define L_LEN 100
    char line[L_LEN];
    int maxWaveformLength = 512;
    
    DisableBreakOnLibraryErrors ();
    
    // initialisieren data
    *pointList = (double *) malloc (maxWaveformLength * sizeof (double));
    
    // Datei lesen
    if ((file = OpenFile (filename, VAL_READ_ONLY, VAL_OPEN_AS_IS, VAL_ASCII)) < 0) {
        displayFileError (filename);
		free(*pointList);
		*pointList = NULL;
		*nPoints = 0;
        return -1;
    }
    i = 0;
    fileEnd = 0;
    while (!fileEnd) {
	    if (i >= maxWaveformLength) {
	        maxWaveformLength *= 2;
	        *pointList = (double *) realloc (*pointList, maxWaveformLength*sizeof(double));
		}	        
        switch (ReadLine (file, line, L_LEN-1)) {
            case -1: 
                displayFileError (filename);
            case -2: 
                fileEnd = 1;
                break;
        }
        if (!fileEnd) {
	        if ((komma = strchr (line, ',')) != NULL) *komma = '.';
	        Scan (line, "%s>%f",  &dval);
			if (dval > max) dval = max;
			if (dval < min) dval = min;           
	        (*pointList)[i] = dval;
	        i++;
	    }
    }
    CloseFile (file);
        
    // zu viele oder gar keine Punkte
    *nPoints = i;
    if (i == 0) {
        free (*pointList);
        *pointList = NULL;
        return 0;
    }
       
    // neue Gr��e f�r Wellenform setzen 
    *pointList = (double *) realloc (*pointList, (*nPoints)*sizeof(double));
	
    return 0;
    
    #undef L_LEN
	
}


// for WFM_TYPE_FILE
int WFM_loadFromFile (t_sequence *seq, t_waveform *new)
{
    return WFM_pointlist_from_file(&new->uList,&new->nPoints,new->filename, WFM_FROM_FILE_MIN_VALUE, WFM_FROM_FILE_MAX_VALUE);
}







/************************************************************************

    converts a sequence file from an old program version 
    (prior to V1.60)

    In the old dataformat
    -  each section was ended with "[END]"
    -  Waveform were not numbered (just "[Waveform]" instead of "[Waveform1]", 
       "[Waveform2]", "[Waveform3]", ...
    -  same for "[AnalogSequence]" (is now named "[AnalogBlock]") and 
       "[DigitalBlock]"
    -  Waveforms, typ WFM_TYPE_FILE, contained tags "V=xxx", "V=xxx" etc
    	now -> V1=xxx, V2=xxx (or more compact using INI_PutDataArray (...)
    -  same for "Voltage=xxx" and "Interval=xxx" for WFM_TYPE_POINTS
   

/************************************************************************/


int convertSequenceFile_OldPrgVersion (const char *oldFilename, const char *newFilename)
{
	
	int nDigitalBlocks, nAnalogBlocks, nWaveforms;
	int voltageNr, intervalNr;
	int err;
	char *comment, *slashPos;

	int oldFile, newFile;
	#define BUFSIZE 500
	char line[BUFSIZE], newLine[BUFSIZE];
    
    // open old and new file
    if ((oldFile = OpenFile (oldFilename, VAL_READ_ONLY, VAL_OPEN_AS_IS, VAL_ASCII)) < 0) {
		displayFileError (oldFilename);
		return -1;
	}	
    if ((newFile = OpenFile (newFilename, VAL_WRITE_ONLY, VAL_TRUNCATE,
							 VAL_ASCII)) < 0) {
		displayFileError (newFilename);
		CloseFile (oldFile);
		return -1;
	}	

	// add new section "Fileinfo" 
	sprintf (line, "[%s]", INI_SECTION_FILEINFO);
	WriteLine (newFile, line, -1);
	sprintf (line, "%s=%1.2f", INI_TAG_FILEINFO_Version, ProgramVersion);
	WriteLine (newFile, line, -1);

	// reset counters
	nDigitalBlocks 	= 0;
	nAnalogBlocks  	= 0;
	nWaveforms     	= 0;
	voltageNr 		= 0;
	intervalNr 		= 0;

	while ((err = ReadLine (oldFile, line, BUFSIZE-1)) >= 0) {
		// check for [END] and remove it
		if (stricmp (line, INI_OLD_SECTION_END) == 0) {
			line[0] = 0;
		}
		// check for [waveform] and add waveform number
		else if (stricmp (line, INI_OLD_SECTION_Waveform) == 0) {
			sprintf (line+1, INI_SECTION_WFM, nWaveforms+1);
			nWaveforms++;
			strcat (line, "]");
			voltageNr = 0;
			intervalNr = 0;
		} 
		else if (strncmp (line, INI_OLD_TAG_WFM_Voltage, strlen(INI_OLD_TAG_WFM_Voltage)) == 0) {
			sprintf (newLine, INI_TAG_WFM_Voltage, voltageNr);
			voltageNr ++;
			strcat (newLine, line+7);
			strcpy (line, newLine);
		}
		else if (strncmp (line, INI_OLD_TAG_WFM_Interval, strlen (INI_OLD_TAG_WFM_Interval)) == 0) {
			sprintf (newLine, INI_TAG_WFM_Interval, intervalNr);
			intervalNr ++;
			strcat (newLine, line+8);
			strcpy (line, newLine);
		}
		else if (strncmp (line, INI_OLD_TAG_WFM_V, strlen(INI_OLD_TAG_WFM_V)) == 0) {
			sprintf (newLine, INI_TAG_WFM_V, voltageNr);
			voltageNr ++;
			strcat (newLine, line+1);
			strcpy (line, newLine);
		}
		// ---------------------------------
		// 		 check for [AnalogSequence]...
		// ----------------------------------
		else if (stricmp (line, INI_OLD_SECTION_AnalogBlock) == 0) {
			sprintf (line+1, INI_SECTION_ANALOGBLOCK, nAnalogBlocks+1);
			nAnalogBlocks++;
			strcat (line, "]");
		} 
		// ---------------------------------
		// 		 check for [DigitalBlock]...
		// ----------------------------------
		else if (stricmp (line, INI_OLD_SECTION_DigitalBlock) == 0) {
			sprintf (line+1, INI_SECTION_DIGITALBLOCK, nDigitalBlocks+1);
			nDigitalBlocks++;
			strcat (line, "]");
		} 
		// remove path in Tag "Filname=.."
		else if (stricmp (line, INI_OLD_TAG_WFM_Filename) == 0) {
			slashPos = strrchr (line, '\\');
			if (slashPos != NULL) {
			    sprintf (newLine, "%s=\"%s", INI_OLD_TAG_WFM_Filename, slashPos+1);
			    strcpy (line, newLine);
			}
		} 
		comment = strchr (line, '#');
		if (comment != NULL) comment[0] = 0;
		
		WriteLine (newFile, line, -1);
	}

	// add number of items to [SequenceInfo]
	sprintf (line, "[%s]", INI_SECTION_SEQUENCEINFO);
	WriteLine (newFile, line, -1);
	sprintf (line, "%s=%d", INI_TAG_SEQUENCEINFO_nDigitalBlocks, nDigitalBlocks);
	WriteLine (newFile, line, -1);
	sprintf (line, "%s=%d", INI_TAG_SEQUENCEINFO_nWaveforms, nWaveforms);
	WriteLine (newFile, line, -1);

	CloseFile (oldFile);
	CloseFile (newFile);
	return 0;
}






/************************************************************************

    writes a sequence file in the old program version 
    (prior to V1.60)

    In the old dataformat
    -  each section was ended with "[END]"
    -  Waveform were not numbered (just "[Waveform]" instead of "[Waveform1]", 
       "[Waveform2]", "[Waveform3]", ...
    -  same for "[AnalogSequence]" (is now named "[AnalogBlock]") and 
       "[DigitalBlock]"
    -  Waveforms, typ WFM_TYPE_FILE, contained tags "V=xxx", "V=xxx" etc
    	now -> V1=xxx, V2=xxx (or more compact using INI_PutDataArray (...)
    -  same for "Voltage=xxx" and "Interval=xxx" for WFM_TYPE_POINTS
   

/************************************************************************/

			   
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// 	 functions to maintain compatibility with old prg version (prior to V1.60)
//   (called by function "saveSequence_OldPrgVersion(...)" )
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

/*
int writeWaveform_OldPrgVersion (FILE *f, t_sequence *seq, int nr)
{
    int i;
    t_waveform *wfm;
    double sumVal = 0;

	if ((wfm = WFM_ptr(seq, nr)) == NULL) return -1;
	fprintf (f, "############### Waveform Nr. %d\n", nr);
	fprintf (f, "%s\n", INI_OLD_SECTION_Waveform);
	fprintf (f, "%s=\"%s\"\n", INI_TAG_WFM_Name, wfm->name);
	switch (wfm->type) {
	    case WFM_TYPE_POINTS:
			fprintf (f, "%s=%u\n", INI_TAG_WFM_NumberOfPoints, wfm->nPoints);
            for (i=0; i < wfm->nPoints-1; i++) {
                fprintf (f, "%s%f\n", INI_OLD_TAG_WFM_Voltage, wfm->uList[i]);
                fprintf (f, "%s%u\n", INI_OLD_TAG_WFM_Interval, 
                		 RoundRealToNearestInteger ((wfm->points[i].timeStart - sumVal) / VAL_MIN_ns));
                sumVal +=  wfm->points[i].timeStart;
            }
			fprintf (f, "%s%f\n", INI_OLD_TAG_WFM_Voltage, wfm->uList[wfm->nPoints-1]);
			break;
	    case WFM_TYPE_FILE:
    		fprintf (f, "%s=\"%s\"\n", INI_TAG_WFM_Filename, wfm->filename);
			fprintf (f, "%s=%f\n", INI_TAG_WFM_uScale, wfm->uScale);
		    fprintf (f, "%s=%f\n", INI_TAG_WFM_offset, wfm->offset);
			fprintf (f, "%s=%u\n", INI_TAG_WFM_tScale, wfm->duration);
			fprintf (f, "%s=%u\n", INI_TAG_WFM_NumberOfPoints, wfm->nPoints);
			// DEBUG: check why "-1"
            for (i=0; i < wfm->nPoints-1; i++) {
                fprintf (f, "%s%f\n", INI_OLD_TAG_WFM_V, wfm->uList[i]);
         	}
			break;
		case WFM_TYPE_STEP:
			fprintf (f, "%s=%f\n", INI_TAG_WFM_stepFrom, wfm->stepFrom);
			fprintf (f, "%s=%f\n", INI_TAG_WFM_stepTo, wfm->stepTo);
			fprintf (f, "%s=%u\n", INI_TAG_WFM_stepRepetition, wfm->stepRep);
			break;	  
		}
	if (wfm->add != WFM_ID_UNCHANGED) fprintf (f, "%s=%u", INI_TAG_WFM_add, wfm->add);
	fprintf (f, "%s\n\n\n", INI_OLD_SECTION_END);
	return 0;
}
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// 	 functions to maintain compatibility with old prg version (prior to V1.60)
//   (called by function "saveSequence_OldPrgVersion(...)" )
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
void writeDACSetup_OldPrgVersion (FILE *f, t_sequence *seq)
{
    int i;
    char name[30];
    
    fprintf (f,"[%s]\n", INI_SECTION_DACSETUP); 
    for (i=0; i < N_AO_CHANNELS; i++) {
    	sprintf (name, INI_TAG_DACSETUP_Name, str_AnalogChannelNames(i));
    	if (seq->AO_ChannelNames[i] != NULL) 
			fprintf (f, "%s=\"%s\"\n", name, seq->AO_ChannelNames[i]);
    } 
	fprintf (f, "%s=%d\n", INI_TAG_DACSETUP_Timebase, seq->AO_timeBase);
	fprintf (f, "%s=%d\n", INI_TAG_DACSETUP_PatternInterval, seq->AO_patternInterval);
	fprintf (f, "%s=%d\n", INI_TAG_DACSETUP_Trigger, seq->trigger);
	fprintf (f, "%s\n\n\n", INI_OLD_SECTION_END);
}
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// 	 functions to maintain compatibility with old prg version (prior to V1.60)
//   (called by function "saveSequence_OldPrgVersion(...)" )
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
void writeDIOSetup_OldPrgVersion (FILE *f, t_sequence *seq)
{
    int i;
    char tagName[30];

    fprintf (f,"[%s]\n", INI_SECTION_DIOSETUP); 
	fprintf (f, "%s=%d\n", INI_TAG_DIOSETUP_Timebase, 		 4);
	fprintf (f, "%s=%d\n", INI_TAG_DIOSETUP_PatternInterval, seq->DIO_patternInterval);
	fprintf (f, "%s=%d\n", INI_TAG_DIOSETUP_OutputPorts, 	 4);
	fprintf (f, "%s=%d\n", INI_TAG_DIOSETUP_nChannels, 	 	 seq->maxDigitalChannel);
    for (i=0; i <  seq->maxDigitalChannel; i++) {
    	if (seq->DIO_ChannelNames[i] != NULL) {
    		sprintf (tagName, INI_TAG_DIOSETUP_Name, str_ChannelNames(i));
			fprintf (f, "%s=\"%s\"\n", tagName, seq->DIO_ChannelNames[i]);
		}
    } 
	fprintf (f, "%s\n\n\n", INI_OLD_SECTION_END);
}
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// 	 functions to maintain compatibility with old prg version (prior to V1.60)
//   (called by function "saveSequence_OldPrgVersion(...)" )
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
*/
/*
void writeAnalogBlock_OldPrgVersion (FILE *f, t_sequence *seq, int nr)
{
    t_analogBlock *new;
    char tagName[30];
    int i;
	
    new = ANALOGBLOCK_ptr (seq, nr);
    fprintf (f, "%s\n", INI_OLD_SECTION_AnalogBlock); 
	if (new->name != 0) {
		fprintf (f, "%s=\"%s\"\n", INI_TAG_ANALOGBLOCK_Name, new->name);
    }
    for (i=0; i < N_AO_CHANNELS; i++) {
    	if (new->waveforms[i] != WFM_ID_UNCHANGED) {
	    	sprintf (tagName, INI_TAG_ANALOGBLOCK_Waveform, str_AnalogChannelNames(i));
	        fprintf (f, "%s=%d\n", tagName, new->waveforms[i]);
	    }
    }
	fprintf (f, "%s\n\n\n", INI_OLD_SECTION_END);
    
}


*/
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// 	 functions to maintain compatibility with old prg version (prior to V1.60)
//   (called by function "saveSequence_OldPrgVersion(...)" )
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

/*
void writeDigitalBlock_OldPrgVersion (FILE *f, t_sequence *seq, int nr)
{
    
    t_digitalBlock *new;
    char tagName[30];
    int i;
	
    new = DIGITALBLOCK_ptr (seq, nr);
    fprintf (f,"%s\n", INI_OLD_SECTION_DigitalBlock); 
	if (new->name != 0) {
		fprintf (f, "%s=\"%s\"\n", INI_TAG_DIGITALBLOCK_Name,     new->name);
	}
	fprintf (f, "%s=%u\n", INI_TAG_DIGITALBLOCK_Time,             new->duration);
	fprintf (f, "%s=%d\n", INI_OLD_TAG_DIGITALBLOCK_AnalogBlock,  new->analogBlock);
	fprintf (f, "%s=%d\n", INI_TAG_DIGITALBLOCK_BlockMode, 		  new->blockMode);
	fprintf (f, "%s=%d\n", INI_TAG_DIGITALBLOCK_VariableTime,     new->variableTime);
	fprintf (f, "%s=%d\n",  INI_TAG_DIGITALBLOCK_IncrementTime,   new->incrementTime);
	fprintf (f, "%s=%d\n", INI_TAG_DIGITALBLOCK_VariableTimeRep,  new->variableTimeRep);
	fprintf (f, "%s=%d\n", INI_TAG_DIGITALBLOCK_VariableTimeStep, new->variableTimeStep);
    for (i=0; i < seq->maxDigitalChannel; i++) {
    	if (new->channels[i] != 0) {
	    	sprintf (tagName, INI_TAG_DIGITALBLOCK_Ch, str_ChannelNames(i));
	        fprintf (f, "%s=%d\n", tagName, new->channels[i]);
	    }
    }
	fprintf (f, "%s\n\n\n", INI_OLD_SECTION_END);
}
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// 	 functions to maintain compatibility with old prg version (prior to V1.60)
//   (called by function "saveSequence_OldPrgVersion(...)" )
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


int saveSequenceFile_OldPrgVersion (const char *filename, t_sequence *seq)  
{
	FILE *f;
	int i;
	
	f = fopen (filename, "w");
	
	writeDACSetup_OldPrgVersion (f, seq);
	writeDIOSetup_OldPrgVersion (f, seq);

	for (i = 0; i < ListNumItems (seq->lWaveforms); i++) 
		writeWaveform_OldPrgVersion (f, seq, i+1);
	
//	for (i = 0; i < ListNumItems (seq->lAnalogBlocks); i++) 
//		writeAnalogBlock_OldPrgVersion (f, seq, i+1);

	for (i = 0; i < ListNumItems (seq->lDigitalBlocks); i++) 
		writeDigitalBlock_OldPrgVersion (f, seq, i+1);
		
	fclose (f); 
	
	return 0;

}

*/



unsigned long DIOSETUP_getTimeUnits (int timebase)
{
   const unsigned long unit[6] =    // times in units of 
                                    // SMALLEST_UNIT (= 50 ns)
       {     VAL_50ns,      //  50 ns
               VAL_us,      //   1 �s
            10*VAL_us,      //  10 �s
           100*VAL_us,      // 100 �s
               VAL_ms,      //   1 ms
            10*VAL_ms};     //  10 ms
   
   
   if (timebase == -3) return unit[0];
   if ((timebase < 1) || (timebase > 5)) return 0;
   return unit[timebase];
}



/******************************************************************************/
/*
/*   FUNCTIONS: Ini_GetDACSetup (...)
/*			    Ini_PutDACSetup (...)
/*
/*    Reads/writes configuration for analog output channels 
/*    in Section "[DAC_Setup...]" from/to "ini-style"-file
/*    --  called by function "loadSequence(...), "saveSequence(...)", 
/*	  --  uses datatype "IniText" defined in Libaray "inifile.c"
/*
/******************************************************************************/

void Ini_GetDACSetup (IniText ini, t_sequence *seq)
{
    int i;
    char tagName[30];
	int oldPatternInterval, oldTimebase; 
	

    for (i=0; i < N_DAC_CHANNELS; i++) {
    	sprintf (tagName, INI_TAG_DACSETUP_Name, str_AnalogChannelNames(i));
		Ini_GetStringCopy (ini, INI_SECTION_DACSETUP, tagName, &seq->AO_channelNames[i]);
    } 

	Ini_GetInt (ini, INI_SECTION_DACSETUP,  INI_TAG_DACSETUP_nChannels, 
				&seq->maxAnalogChannels);
	if (seq->maxAnalogChannels > N_DAC_CHANNELS) seq->maxAnalogChannels = N_DAC_CHANNELS;


	if (Ini_GetInt (ini, INI_SECTION_DACSETUP, INI_TAG_DACSETUP_Timebase_50ns,
			    &seq->AO_timebase_50ns) == 0) {
		Ini_GetInt (ini, INI_SECTION_DACSETUP, INI_TAG_DACSETUP_Timebase,
					 &oldTimebase);
		Ini_GetInt (ini, INI_SECTION_DACSETUP, INI_TAG_DACSETUP_PatternInterval, 
					 &oldPatternInterval);
		seq->AO_timebase_50ns = DIOSETUP_getTimeUnits (oldTimebase) * oldPatternInterval;
		OUTPUTDATA_getTimebases (seq);
	}
		

	Ini_GetInt (ini, INI_SECTION_DACSETUP,  INI_TAG_DACSETUP_Trigger,
				 &seq->trigger);
	Ini_GetInt (ini, INI_SECTION_DACSETUP,  INI_TAG_DACSETUP_triggerPolarity,
				&seq->triggerPolarity);
	//if (seq->trigger == ND_PFI_3) seq->trigger = ND_PFI_2;


	Ini_GetDataArrayEx (ini, INI_SECTION_DACSETUP, INI_TAG_DACSETUP_tableRowHeight,
						seq->DAC_tableRowHeight, N_DAC_CHANNELS, VAL_INTEGER);
//	Ini_GetInt (ini, INI_SECTION_DACSETUP, INI_TAG_DACSETUP_tableHeight,
//				&seq->DAC_tableHeight);
}

/*****************************************************************************/



void Ini_PutDACSetup (IniText ini, t_sequence *seq)
{
    int i;
    char tagName[30];
	int oldPatternInterval, oldTimebase; 

    for (i=0; i < N_DAC_CHANNELS; i++) {
    	sprintf (tagName, INI_TAG_DACSETUP_Name, str_AnalogChannelNames(i));
    	if (seq->AO_channelNames[i] != NULL) 
			Ini_PutString (ini, INI_SECTION_DACSETUP, tagName, seq->AO_channelNames[i]);
    } 

	
	Ini_PutInt (ini, INI_SECTION_DACSETUP,  INI_TAG_DACSETUP_nChannels, 
				seq->maxAnalogChannels);
	

	Ini_PutInt (ini, INI_SECTION_DACSETUP, INI_TAG_DACSETUP_Timebase_50ns,
			    seq->AO_timebase_50ns);
	// keep data compatible with versions prior to 4.20
	oldTimebase = 1; // VAL_us
	oldPatternInterval = seq->AO_timebase_50ns / DIOSETUP_getTimeUnits (oldTimebase);
	Ini_PutInt (ini, INI_SECTION_DACSETUP, INI_TAG_DACSETUP_Timebase,
				 oldTimebase);
	Ini_PutInt (ini, INI_SECTION_DACSETUP, INI_TAG_DACSETUP_PatternInterval, 
				 oldPatternInterval);
	
	Ini_PutInt (ini, INI_SECTION_DACSETUP,  INI_TAG_DACSETUP_Trigger,
				 seq->trigger);
//	if (seq->triggerPolarity != ND_LOW_TO_HIGH) { 				 
//		Ini_PutInt (ini, INI_SECTION_DACSETUP,  INI_TAG_DACSETUP_triggerPolarity,
//				 seq->triggerPolarity);
//	}

	Ini_PutDataArray (ini, INI_SECTION_DACSETUP, INI_TAG_DACSETUP_tableRowHeight,
				seq->DAC_tableRowHeight, N_DAC_CHANNELS, VAL_INTEGER);
//	Ini_PutInt (ini, INI_SECTION_DACSETUP, INI_TAG_DACSETUP_tableHeight,
//				seq->DAC_tableHeight);

}


/******************************************************************************/
/*
/*   FUNCTIONS: Ini_GetDIOSetup (...)
/*			    Ini_PutDIOSetup (...)
/*
/*    Reads/writes configuration for analog output channels 
/*    in Section "[DAC_Setup...]" from/to "ini-style"-file
/*    --  called by function "loadSequence(...), "saveSequence(...)", 
/*	  --  uses datatype "IniText" defined in Libaray "inifile.c"
/*
/******************************************************************************/


void Ini_GetDIOSetup (IniText ini, t_sequence *seq)
{
    int i;
    char tagName[30];

	int oldPatternInterval, oldTimebase; 

	if (Ini_GetInt (ini, INI_SECTION_DIOSETUP, INI_TAG_DIOSETUP_Timebase_50ns,
			    &seq->DIO_timebase_50ns) == 0) {
		Ini_GetInt (ini, INI_SECTION_DIOSETUP, INI_TAG_DIOSETUP_Timebase,
					 &oldTimebase);
		Ini_GetInt (ini, INI_SECTION_DIOSETUP, INI_TAG_DIOSETUP_PatternInterval, 
					 &oldPatternInterval);
		seq->DIO_timebase_50ns = DIOSETUP_getTimeUnits (oldTimebase) * oldPatternInterval;
	}
	Ini_GetInt (ini, INI_SECTION_DIOSETUP,  INI_TAG_DIOSETUP_nChannels, 
				&seq->maxDigitalChannel);
	if (seq->maxDigitalChannel > N_TOTAL_DIO_CHANNELS) seq->maxDigitalChannel = N_TOTAL_DIO_CHANNELS;
    for (i=0; i < N_TOTAL_DIO_CHANNELS; i++) {
    	sprintf (tagName, INI_TAG_DIOSETUP_Name, str_ChannelNames(i));
		Ini_GetStringCopy (ini, INI_SECTION_DIOSETUP, tagName, &seq->DIO_channelNames[i]);
    } 
    Ini_GetDataArrayEx (ini, INI_SECTION_DIOSETUP, INI_TAG_DIOSETUP_ChannelStatus, 
    	seq->DIO_channelStatus, N_TOTAL_DIO_CHANNELS, VAL_INTEGER);
    Ini_GetDataArrayEx_Comp (ini, INI_SECTION_DIOSETUP, INI_TAG_DIOSETUP_invertDisplay, 
	    	seq->DIO_invertDisplay, N_TOTAL_DIO_CHANNELS, VAL_INTEGER);
    Ini_GetDataArrayEx_Comp (ini, INI_SECTION_DIOSETUP, INI_TAG_DIOSETUP_lockChannel, 
	    	seq->DIO_lockChannel, N_TOTAL_DIO_CHANNELS, VAL_INTEGER);
	
	
}
/******************************************************************************/
    

void Ini_PutDIOSetup (IniText ini, t_sequence *seq)
{
    int i;
    char tagName[30];

	int oldPatternInterval, oldTimebase; 

	

	Ini_PutInt (ini, INI_SECTION_DIOSETUP, INI_TAG_DIOSETUP_Timebase_50ns,
			    seq->DIO_timebase_50ns);
	// keep data compatible with versions prior to 4.20
	oldTimebase = -3; // VAL_50ns
	oldPatternInterval = seq->DIO_timebase_50ns / DIOSETUP_getTimeUnits (oldTimebase);
	Ini_PutInt (ini, INI_SECTION_DIOSETUP, INI_TAG_DIOSETUP_Timebase,
				 oldTimebase);
	Ini_PutInt (ini, INI_SECTION_DIOSETUP, INI_TAG_DIOSETUP_PatternInterval, 
				 oldPatternInterval);

	Ini_PutInt (ini, INI_SECTION_DIOSETUP,  INI_TAG_DIOSETUP_OutputPorts, 4);
	Ini_PutInt (ini, INI_SECTION_DIOSETUP,  INI_TAG_DIOSETUP_nChannels, 
				seq->maxDigitalChannel);

  	if (isArrayNotEqual_int (seq->DIO_channelStatus, N_TOTAL_DIO_CHANNELS, 0)) {
	    Ini_PutDataArray (ini, INI_SECTION_DIOSETUP, INI_TAG_DIOSETUP_ChannelStatus, 
	    	seq->DIO_channelStatus, N_TOTAL_DIO_CHANNELS, VAL_INTEGER);
	}

   	if (isArrayNotEqual_int (seq->DIO_invertDisplay, N_TOTAL_DIO_CHANNELS, 0)) {
	    Ini_PutDataArray_Comp (ini, INI_SECTION_DIOSETUP, INI_TAG_DIOSETUP_invertDisplay, 
	    	seq->DIO_invertDisplay, N_TOTAL_DIO_CHANNELS, VAL_INTEGER);
	}

    Ini_PutDataArray_Comp (ini, INI_SECTION_DIOSETUP, INI_TAG_DIOSETUP_lockChannel, 
	    	seq->DIO_lockChannel, N_TOTAL_DIO_CHANNELS, VAL_INTEGER);
	
    for (i=0; i < seq->maxDigitalChannel; i++) {
    	if (seq->DIO_channelNames[i] != NULL) {
    		sprintf (tagName, INI_TAG_DIOSETUP_Name, str_ChannelNames(i));
			Ini_PutString (ini, INI_SECTION_DIOSETUP, tagName, seq->DIO_channelNames[i]);
		}
    } 
}



void Ini_PutDMDImage (IniText ini, char *sectionName, t_DMDimage *d)
{
	if (d == NULL) return;
	
	Ini_PutString (ini, sectionName, INI_TAG_WFM_DMD_shapeID, DMDSHAPE_identifier (d->shapeID));
	if (d->shapeID > 0) {
		Ini_PutDataArray (ini, sectionName, INI_TAG_WFM_DMD_shapeParametersFrom, d->shapeParametersFrom, 
						  DMDSHAPE_nParameters (d->shapeID), VAL_DOUBLE);
		Ini_PutDataArray (ini, sectionName, INI_TAG_WFM_DMD_shapeParametersTo, d->shapeParametersTo, 
						  DMDSHAPE_nParameters (d->shapeID), VAL_DOUBLE);
		Ini_PutDataArray (ini, sectionName, INI_TAG_WFM_DMD_shapeParametersVary, d->shapeParametersVary, 
						  DMDSHAPE_nParameters (d->shapeID), VAL_INTEGER);
		Ini_PutPoint (ini, sectionName, INI_TAG_WFM_DMD_bitmapSize, d->bitmapSize);
	}
	Ini_PutInt (ini, sectionName, INI_TAG_WFM_DMD_enable, d->enableDMD); 
	Ini_PutInt (ini, sectionName, INI_TAG_WFM_DMD_enableDMDPhaseFeedback, d->enableDMDPhaseFeedback); 
	Ini_PutInt (ini, sectionName, INI_TAG_WFM_DMD_enableDMDErrorDiffusion, d->enableDMDErrorDiffusion);
	Ini_PutInt (ini, sectionName, INI_TAG_WFM_DMD_enableDMDSmoothing, d->enableDMDSmoothing);
	
	
}



void Ini_GetDMDImage (IniText ini, char *sectionName, t_DMDimage *d)
{
	char temp[200];
	
	if (d == NULL) return;
	
	Ini_GetStringIntoBuffer (ini, sectionName, INI_TAG_WFM_DMD_shapeID, temp, 199);
	d->shapeID = DMDSHAPE_idFromIdentifier (temp);
	if (d->shapeID > 0) {
		DMDIMAGE_allocateMemory (d);
		Ini_GetDataArrayEx (ini, sectionName, INI_TAG_WFM_DMD_shapeParametersFrom, d->shapeParametersFrom, 
						  DMDSHAPE_nParameters (d->shapeID), VAL_DOUBLE);
		Ini_GetDataArrayEx (ini, sectionName, INI_TAG_WFM_DMD_shapeParametersTo, d->shapeParametersTo, 
						  DMDSHAPE_nParameters (d->shapeID), VAL_DOUBLE);
		Ini_GetDataArrayEx (ini, sectionName, INI_TAG_WFM_DMD_shapeParametersVary, d->shapeParametersVary, 
						  DMDSHAPE_nParameters (d->shapeID), VAL_INTEGER);
//		Ini_GetPoint (ini, sectionName, INI_TAG_WFM_DMD_bitmapSize, d->bitmapSize);
		
	}
	Ini_GetInt (ini, sectionName, INI_TAG_WFM_DMD_enable, &d->enableDMD);  
	Ini_GetInt (ini, sectionName, INI_TAG_WFM_DMD_enableDMDPhaseFeedback, &d->enableDMDPhaseFeedback); 
	Ini_GetInt (ini, sectionName, INI_TAG_WFM_DMD_enableDMDErrorDiffusion, &d->enableDMDErrorDiffusion); 
	Ini_GetInt (ini, sectionName, INI_TAG_WFM_DMD_enableDMDSmoothing, &d->enableDMDSmoothing); 
	
}



/******************************************************************************/
/*
/*   FUNCTIONS: Ini_GetExternalDevice (...)
/*			    Ini_PutExternalDevice (...)
/*
/*    Reads/writes data for a waveform in 
/*    in Section "[External Devices...]" from/to "ini-style"-file
/*    --  called by function "loadSequence(...), "saveSequence(...)", 
/*	  --  uses datatype "IniText" defined in Libaray "inifile.c"
/*
/******************************************************************************/

void Ini_PutExternalDevice (IniText ini, t_sequence *seq, int nr)
{
	char sectionName[30];
    t_ext_device *extdev;

	extdev = EXTDEV_ptr(seq, nr);
	if (extdev == NULL){
		tprintf("Error: invalid device pointer\n");
		return;
	}
	if ((extdev==seq->noExternalDevice) || (extdev->deviceTyp == EXTDEV_TYPE_NONE)) {
		return;	
	}

	
	sprintf (sectionName, INI_SECTION_EXTDEV, nr);

	Ini_PutUInt (ini, sectionName, INI_TAG_EXTDEV_type, extdev->deviceTyp);
	Ini_PutUInt (ini, sectionName, INI_TAG_EXTDEV_uniqueID, extdev->uniqueID);
	Ini_PutString (ini, sectionName, INI_TAG_EXTDEV_Name, extdev->name);

	switch (extdev->deviceTyp) {
    	case EXTDEV_TYPE_NONE:
			break;
    	case EXTDEV_TYPE_TELNET:
			Ini_PutString (ini, sectionName, INI_TAG_EXTDEV_address, extdev->address);
			Ini_PutInt0 (ini, sectionName, INI_TAG_EXTDEV_port, extdev->port);
			break;
	}
	
}

/******************************************************************************/


int Ini_GetExternalDevice (IniText ini, t_sequence *seq, int nr)
{
	char sectionName[30];
//	char functionParameterListTag[200]; 
//	char tagName[30];
    t_ext_device *new;
    //unsigned long dummy,dummy2,dummy3;
	//unsigned long addrMatrixArraySize;
   // int i,j;
    int lastFound;
	char temp[200];
//	double *tmpFrom;
//	double *tmpTo;
//	int *tmpStepType;
	//t_function *f;
//	int nParams;
	unsigned int dummyID;
	int found;
	
	//DisableBreakOnLibraryErrors ();
	
	sprintf (sectionName, INI_SECTION_EXTDEV, nr);

	found = Ini_GetStringIntoBuffer (ini, sectionName, INI_TAG_EXTDEV_Name, temp, MAX_EXT_DEV_NAME_LEN);
	
	if (!found) {
		return -1;	
	}
	// get type of external device
	new = EXTDEV_new (seq);
    EXTDEV_init (new, EXTDEV_TYPE_NONE);
	strcpy(new->name,temp);
	Ini_GetInt (ini, sectionName, INI_TAG_EXTDEV_type, &new->deviceTyp);
	
	//dummyID=time;
	//EXTDEV_checkForDuplicateIdandChange (seq, &dummyID);
	//new->uniqueID=dummyID;
	Ini_GetInt (ini, sectionName, INI_TAG_EXTDEV_uniqueID, &dummyID);
	if (EXTDEV_checkForDuplicateIdandChange (seq, &dummyID)){
		tprintf("Error: ID has been changed during loading\n");
	}
	new->uniqueID=dummyID;
	switch (new->deviceTyp) {
	    case EXTDEV_TYPE_NONE:
	    	break;
	    case EXTDEV_TYPE_TELNET:
	    	Ini_GetStringIntoBuffer (ini, sectionName, INI_TAG_EXTDEV_address, new->address, 256);
			Ini_GetInt (ini, sectionName, INI_TAG_EXTDEV_port, &new->port);
			break;
	}
	return 0;
};




/******************************************************************************/
/*
/*   FUNCTIONS: Ini_GetWaveform (...)
/*			    Ini_PutWaveform (...)
/*
/*    Reads/writes data for a waveform in 
/*    in Section "[Waveform...]" from/to "ini-style"-file
/*    --  called by function "loadSequence(...), "saveSequence(...)", 
/*	  --  uses datatype "IniText" defined in Libaray "inifile.c"
/*
/******************************************************************************/

void Ini_PutWaveform (IniText ini, t_sequence *seq, int nr)
{
	char sectionName[30];
	char functionParameterListTag[200];
	
    t_waveform *wfm;

    double* tmpFrom;
	double* tmpTo;
	int* tmpStepType;
    double last;
    int i,j;

	sprintf (sectionName, INI_SECTION_WFM, nr);
	wfm = WFM_ptr(seq, nr);
	
	Ini_PutUInt   (ini, sectionName, INI_TAG_WFM_type, wfm->type);
	Ini_PutString (ini, sectionName, INI_TAG_WFM_Name, wfm->name);
	Ini_PutInt   (ini, sectionName, INI_TAG_WFM_channel, wfm->channel);
	if (wfm->timebaseStretch != 1) Ini_PutInt (ini, sectionName, INI_TAG_WFM_timebaseStretch, wfm->timebaseStretch);
	

	if (wfm->hide) Ini_PutInt (ini, sectionName, INI_TAG_WFM_hide, wfm->hide);
	
	if (wfm->transferFunction > 0)
		Ini_PutInt (ini, sectionName, INI_TAG_WFM_transferFunction, wfm->transferFunction);
	if (strlen (wfm->strValueName) > 0) {
		Ini_PutString (ini, sectionName, INI_TAG_WFM_strValueName,
					   wfm->strValueName);
	}
	if (strlen (wfm->strValueUnits) > 0) {
		Ini_PutString (ini, sectionName, INI_TAG_WFM_strValueUnits,
					   wfm->strValueUnits);
	}
	

	switch (wfm->type) {
	    case WFM_TYPE_POINTS:
	    	last = 0;
			Ini_PutUInt (ini, sectionName, INI_TAG_WFM_NumberOfPoints,
						 wfm->nPoints);
           	
/*           	iList = (unsigned *) malloc (wfm->nPoints * sizeof (unsigned));
           	for (i = 0; i < wfm->nPoints-1; i++) {
           		iList[i] = RoundRealToNearestInteger ((wfm->points[i+1].timeStart - wfm->points[i].timeStart) / VAL_MIN_ns);
           	}
           	if (wfm->nPoints > 0) iList[wfm->nPoints-1] = 0;
           	Ini_PutDataArray (ini, sectionName, INI_TAG_WFM_iList, 
           					  iList, wfm->nPoints, VAL_UNSIGNED_INTEGER);
           	free (iList);
*/           	
			for (i = 0; i < wfm->nPoints; i++) {
				Ini_PutTPoint (ini, sectionName, &wfm->points[i], i);
			}
			break;
	    case WFM_TYPE_FILE:
    		Ini_PutString (ini, sectionName, INI_TAG_WFM_Filename, wfm->filename);
			
			Ini_PutDouble (ini, sectionName, INI_TAG_WFM_uScaleFrom, wfm->uScaleFrom);
			Ini_PutDouble (ini, sectionName, INI_TAG_WFM_uScaleTo, wfm->uScaleTo);
			Ini_PutInt0 (ini, sectionName, INI_TAG_WFM_uScaleVary, wfm->uScaleVary);

			Ini_PutDouble (ini, sectionName, INI_TAG_WFM_offsetFrom, wfm->offsetFrom);
			Ini_PutDouble (ini, sectionName, INI_TAG_WFM_offsetTo, wfm->offsetTo);
			Ini_PutInt0 (ini, sectionName, INI_TAG_WFM_offsetVary, wfm->offsetVary);
			
			Ini_Put_Int64 (ini, sectionName, INI_TAG_WFM_tScaleFrom, wfm->durationFrom_50ns);
			Ini_Put_Int64 (ini, sectionName, INI_TAG_WFM_tScaleTo, wfm->durationTo_50ns);
			Ini_PutInt0 (ini, sectionName, INI_TAG_WFM_tScaleVary, wfm->durationVary);

			
			Ini_PutUInt (ini, sectionName, INI_TAG_WFM_NumberOfPoints,
						 wfm->nPoints);
           	Ini_PutDataArray (ini, sectionName, INI_TAG_WFM_uList, 
           					  wfm->uList, wfm->nPoints, VAL_DOUBLE);
			break;
		case WFM_TYPE_STEP:
			Ini_PutDouble (ini, sectionName, INI_TAG_WFM_stepFrom, wfm->stepFrom);
			Ini_PutDouble (ini, sectionName, INI_TAG_WFM_stepTo,  wfm->stepTo);
			Ini_PutInt (ini, sectionName, INI_TAG_WFM_stepRepetition, wfm->stepRep);
			Ini_PutInt (ini, sectionName, INI_TAG_WFM_stepChangeMode, wfm->stepChangeMode);
			if (wfm->stepAlternate) 
				Ini_PutInt (ini, sectionName, INI_TAG_WFM_stepAlternate, wfm->stepAlternate);
			break;
		case WFM_TYPE_FUNCTION:
			Ini_PutString (ini, sectionName, INI_TAG_WFM_functionID, FUNCTION_identifier (wfm->functionID));
			if (wfm->functionID > 0) {
				i = wfm->functionID-1;
				tmpFrom = (double*)calloc(FUNCTION_nParameters(wfm->functionID),sizeof(double));
				tmpTo = (double*)calloc(FUNCTION_nParameters(wfm->functionID),sizeof(double)); 
				tmpStepType = (int*)calloc(FUNCTION_nParameters(wfm->functionID),sizeof(int));
				for (j=0; j < FUNCTION_nParameters(wfm->functionID); j++) {
					tmpFrom[j] = wfm->functionParameters[i][j].from;
					tmpTo[j] = wfm->functionParameters[i][j].to;   
					tmpStepType[j] = wfm->functionParameters[i][j].stepType;
				}
				
				Ini_PutDataArray (ini, sectionName, INI_TAG_WFM_functionParametersFrom, tmpFrom, 
								  WFM_nFuncParams(wfm), VAL_DOUBLE);
				Ini_PutDataArray (ini, sectionName, INI_TAG_WFM_functionParametersTo, tmpTo, 
								  WFM_nFuncParams(wfm), VAL_DOUBLE);
				Ini_PutDataArray (ini, sectionName, INI_TAG_WFM_functionParametersVary, tmpStepType, 
								  WFM_nFuncParams(wfm), VAL_INTEGER);

				
				for (j=0; j < FUNCTION_nParameters(wfm->functionID); j++) {
					if (tmpStepType[j] == WFM_STEP_TYPE_LIST) {
						sprintf(functionParameterListTag,INI_TAG_WFM_functionParametersList,j);
						Ini_PutDataArray (ini, sectionName, functionParameterListTag, wfm->functionParameters[i][j].list, 
								  wfm->functionParameters[i][j].listSize, VAL_DOUBLE);
					}
				}
				
				free(tmpFrom);
				free(tmpTo);
				free(tmpStepType);
				
				
				Ini_Put_Int64 (ini, sectionName, INI_TAG_WFM_tScaleFrom, wfm->durationFrom_50ns);
				Ini_Put_Int64 (ini, sectionName, INI_TAG_WFM_tScaleTo, wfm->durationTo_50ns);
				Ini_PutInt (ini, sectionName, INI_TAG_WFM_tScaleVary, wfm->durationVary);
				Ini_PutInt0 (ini, sectionName, INI_TAG_WFM_functionReverseTime, wfm->functionReverseTime);
			}
			break;
		case WFM_TYPE_ADDRESSING:
			Ini_PutInt0 (ini, sectionName, INI_TAG_WFM_addrLinkWfm, wfm->addrLinkWfm);
			Ini_PutInt (ini, sectionName, INI_TAG_WFM_addrType, wfm->addrType);
			Ini_PutDataArray (ini, sectionName, INI_TAG_WFM_addrCalibration, wfm->addrCalibration, WFM_ADDR_N_CALIBRATION_PARAMETERS, VAL_DOUBLE);
			Ini_PutDataArray (ini, sectionName, INI_TAG_WFM_addrCalibrationStepTo, wfm->addrCalibrationStepTo, WFM_ADDR_N_CALIBRATION_PARAMETERS, VAL_DOUBLE);
			Ini_PutDataArray (ini, sectionName, INI_TAG_WFM_addrCalibrationStep, wfm->addrCalibrationStep, WFM_ADDR_N_CALIBRATION_PARAMETERS, VAL_INTEGER);
			Ini_PutPoint (ini, sectionName, INI_TAG_WFM_addrCenter, wfm->addrCenter);
			Ini_PutString0 (ini, sectionName, INI_TAG_WFM_addrTestFilename, wfm->addrTestFilename );
			Ini_PutString0 (ini, sectionName, INI_TAG_WFM_addrTestFilename2, wfm->addrTestFilename2);
			Ini_Put_Int64 (ini, sectionName, INI_TAG_WFM_tScaleFrom, wfm->durationFrom_50ns);
			Ini_PutInt0 (ini, sectionName, INI_TAG_WFM_addrEnablePhaseFeedback, wfm->addrEnablePhaseFeedback);
			Ini_PutInt0 (ini, sectionName, INI_TAG_WFM_addrEnablePhaseFeedback2, wfm->addrEnablePhaseFeedback2);
			Ini_PutInt0 (ini, sectionName, INI_TAG_WFM_addrCamera, wfm->addrCamera);
			Ini_PutInt0 (ini, sectionName, INI_TAG_WFM_addrAddLastOffset, wfm->addrAddLastOffset);
			Ini_PutInt0 (ini, sectionName, INI_TAG_WFM_addrAddLastOffsetNotAtFirst, wfm->addrAddLastOffsetNotAtFirst);
			Ini_PutInt (ini, sectionName, INI_TAG_WFM_addrImageSize, wfm->addrImageSize);
			
			switch (wfm->addrType) {
				case WFM_ADDR_TYPE_TRAJECTORY:
					Ini_PutDataArray (ini, sectionName, INI_TAG_WFM_addrTrajParams, wfm->addrTrajectParams, WFM_ADDR_N_TRAJ_PARAMETERS, VAL_DOUBLE);
					break;
				case WFM_ADDR_TYPE_DMD:
					Ini_PutDMDImage (ini, sectionName, wfm->addrDMDimage);
					Ini_PutDataArray (ini, sectionName, INI_TAG_WFM_addDisorder, wfm->addDisorder, WFM_ADDR_MATRIX_NUM, VAL_INTEGER); 
					Ini_PutDataArray (ini, sectionName, INI_TAG_WFM_disorderSeed, wfm->disorderSeed, WFM_ADDR_MATRIX_NUM, VAL_UNSIGNED_INTEGER);
					Ini_PutDataArray (ini, sectionName, INI_TAG_WFM_disorderTyp, wfm->disorderTyp, WFM_ADDR_MATRIX_NUM, VAL_INTEGER);
					
					Ini_PutInt(ini, sectionName, INI_TAG_WFM_stepAddrMatrix, wfm->stepAddrMatrix);
					Ini_PutInt(ini, sectionName, INI_TAG_WFM_addrMatricesPerRepetition, wfm->addrMatricesPerRepetition);     
					Ini_PutDataArray (ini, sectionName, INI_TAG_WFM_addrMatrix, wfm->addrMatrix, WFM_ADDR_MATRIXDIM*WFM_ADDR_MATRIX_NUM, VAL_UNSIGNED_INTEGER);
					break;
				case WFM_ADDR_TYPE_SINGLESITE:
					Ini_PutInt(ini, sectionName, INI_TAG_WFM_stepAddrMatrix, wfm->stepAddrMatrix);
					Ini_PutInt(ini, sectionName, INI_TAG_WFM_addrMatricesPerRepetition, wfm->addrMatricesPerRepetition);  
					Ini_PutDataArray (ini, sectionName, INI_TAG_WFM_addrMatrix, wfm->addrMatrix, WFM_ADDR_MATRIXDIM*WFM_ADDR_MATRIX_NUM, VAL_UNSIGNED_INTEGER);
					break;
			}
			break;
			
	}
	if (wfm->add != WFM_ID_UNCHANGED) 
		Ini_PutUInt (ini, sectionName, INI_TAG_WFM_add, wfm->add);
	
}

/******************************************************************************/

int Ini_GetWaveform_PointsOld1 (IniText ini, char *sectionName, t_waveform *w)
{

    unsigned niList, nuList;
    unsigned *iList;
    double *uList;
    int i;
    unsigned sum;

	iList = (unsigned long *) Ini_GetDataArray (ini, sectionName, 
			INI_TAG_WFM_iList, &niList, VAL_UNSIGNED_INTEGER);
	uList = (double *) Ini_GetDataArray (ini, sectionName, 
			INI_TAG_WFM_uList, &nuList, VAL_DOUBLE);
	if ((nuList != w->nPoints) || (niList != w->nPoints) 
	  ||(iList == NULL) || (uList == NULL)) {
		MessagePopup ("Error", "Error loading waveform:\n"
							   "Inconsistent number of points");
		Breakpoint();
	}
	else {
   		sum = 0;
   		for (i = 0; i < w->nPoints; i++) {
   			w->points[i].timeStart_ns  = 50.0 * (sum);
   			w->points[i].valueStart = uList[i];
   			sum += iList[i];
   		}
   	}
	free (iList);
	free (uList);
	return 0;
}



// ------------------------------------------
//   convert data from old program version
// ------------------------------------------

int Ini_GetWaveform_PointsOld2 (IniText ini, char *sectionName, t_waveform *w)
{

    unsigned iValue;
    int k;
    unsigned sum;
	char tagName[30];
	
    sum = 0;
    for (k = 0; k < w->nPoints; k++) {
		sprintf (tagName, INI_TAG_WFM_Voltage, k);
    	Ini_GetDouble (ini, sectionName, tagName, &w->points[k].valueStart);
		sprintf (tagName, INI_TAG_WFM_Interval, k);
    	if (Ini_GetUInt (ini, sectionName, tagName, &iValue) <= 0) {
    		w->points[k].timeStart_ns = 50.0 * (sum);
    		sum += iValue;
    	}
    }
    return 0;
}




int Ini_GetWaveform (IniText ini, t_sequence *seq, int nr)
{
	char sectionName[30];
	char functionParameterListTag[200]; 
	char tagName[30];
    t_waveform *new;
    unsigned long dummy,dummy2,dummy3;
	//unsigned long addrMatrixArraySize;
    int i,j;
    int lastFound;
	char temp[200];
	double *tmpFrom;
	double *tmpTo;
	int *tmpStepType;
	//t_function *f;
	int nParams;
	
	
	DisableBreakOnLibraryErrors ();
	
	sprintf (sectionName, INI_SECTION_WFM, nr);

    new = WFM_new (seq);
    WFM_init (new, WFM_TYPE_POINTS);
    
	Ini_GetStringIntoBuffer (ini, sectionName, INI_TAG_WFM_Name, new->name, MAX_WFM_NAME_LEN);
	// get type of waveform
	
	Ini_GetInt   (ini, sectionName, INI_TAG_WFM_channel, &new->channel);
	Ini_GetInt   (ini, sectionName, INI_TAG_WFM_timebaseStretch, &new->timebaseStretch);


	Ini_GetInt (ini, sectionName, INI_TAG_WFM_hide, &new->hide);

	Ini_GetInt (ini, sectionName, INI_TAG_WFM_type, &new->type);
    // compatibility with old versions:
    // if filename registered, assume WFM_FILE	
    if (Ini_GetStringCopy (ini, sectionName, INI_TAG_WFM_Filename,
    				   &new->filename) != 0) {
        new->type = WFM_TYPE_FILE;
    };
	if (Ini_GetDouble (ini, sectionName, INI_TAG_WFM_stepFrom, &new->stepFrom) > 0) {
		new->type = WFM_TYPE_STEP;
	}
	Ini_GetInt (ini, sectionName, INI_TAG_WFM_NumberOfPoints, &new->nPoints);
	
	Ini_GetInt (ini, sectionName, INI_TAG_WFM_transferFunction, &new->transferFunction);
	Ini_GetStringIntoBuffer (ini, sectionName, INI_TAG_WFM_strValueName,
					   		 new->strValueName, MAX_VALUE_NAME_LEN);
	Ini_GetStringIntoBuffer (ini, sectionName, INI_TAG_WFM_strValueUnits,
					   		 new->strValueUnits, MAX_VALUE_UNITS_LEN);
	switch (new->type) {
	    case WFM_TYPE_POINTS:
           	// allocate & init points
           	new->points = (t_point *) realloc (new->points, new->nPoints * sizeof (t_point));
           	for (i = 0; i < new->nPoints; i++) POINT_init (&new->points[i]);
			// get values for points
			if (Ini_ItemExists (ini, sectionName, INI_TAG_WFM_uList)) {
				// load old data format (verion 2.0)
				Ini_GetWaveform_PointsOld1 (ini, sectionName, new);
			} 
			else if (Ini_ItemExists (ini, sectionName, "Voltage0")) {
				// load old data format (prior to version 1.6)
				Ini_GetWaveform_PointsOld2 (ini, sectionName, new);
			} 
			else {
				// load new format (from version 2.10)
				for (i = 0; i < new->nPoints; i++) {
					Ini_GetTPoint (ini, sectionName, &new->points[i], i);
				}
			}
	    	break;
	    case WFM_TYPE_FILE:
           	new->uList = (double *) Ini_GetDataArray (ini, sectionName, 
           				INI_TAG_WFM_uList, &dummy, VAL_DOUBLE);
           	if (new->uList == NULL) {
           	    // ------------------------------------------
           	    //   convert data from old program version (separate points)
           	    // ------------------------------------------
	            new->uList = (double *) malloc (sizeof (double) * new->nPoints);
	            lastFound = -1;
	            for (i = 0; i < new->nPoints; i++) {
            		sprintf (tagName, INI_TAG_WFM_V, i);
            		new->uList[i] = 0;
	            	if (Ini_GetDouble (ini, sectionName, tagName, &new->uList[i]) > 0) lastFound = i;
           		}
           		new->nPoints = lastFound+1;
           	    // ------------------------------------------
           	}
			Ini_GetDouble (ini, sectionName, INI_TAG_WFM_uScaleFrom, &new->uScaleFrom);
			Ini_GetDouble (ini, sectionName, INI_TAG_WFM_uScaleTo, &new->uScaleTo);
			Ini_GetInt (ini, sectionName, INI_TAG_WFM_uScaleVary, &new->uScaleVary);

			Ini_GetDouble (ini, sectionName, INI_TAG_WFM_offsetFrom, &new->offsetFrom);
			Ini_GetDouble (ini, sectionName, INI_TAG_WFM_offsetTo, &new->offsetTo);
			Ini_GetInt (ini, sectionName, INI_TAG_WFM_offsetVary, &new->offsetVary);
			
			Ini_Get_Int64 (ini, sectionName, INI_TAG_WFM_tScaleFrom, &new->durationFrom_50ns);
			Ini_Get_Int64 (ini, sectionName, INI_TAG_WFM_tScaleTo, &new->durationTo_50ns);
			Ini_GetInt (ini, sectionName, INI_TAG_WFM_tScaleVary, &new->durationVary);
			Ini_GetInt (ini, sectionName, INI_TAG_WFM_functionReverseTime, &new->functionReverseTime);
			break;
		case WFM_TYPE_STEP:
			Ini_GetDouble (ini, sectionName, INI_TAG_WFM_stepFrom,  &new->stepFrom);
			Ini_GetDouble (ini, sectionName, INI_TAG_WFM_stepTo,  &new->stepTo);
			Ini_GetInt (ini, sectionName, INI_TAG_WFM_stepRepetition, &new->stepRep);
			Ini_GetInt (ini, sectionName, INI_TAG_WFM_stepChangeMode, &new->stepChangeMode);
			Ini_GetInt (ini, sectionName, INI_TAG_WFM_stepAlternate, &new->stepAlternate);
			break;
		case WFM_TYPE_FUNCTION:
			Ini_GetStringIntoBuffer (ini, sectionName, INI_TAG_WFM_functionID, temp, 199);
			new->functionID = FUNCTION_idFromIdentifier (temp);
			if (new->functionID != 0) {
				WFM_FUNC_allocateMemory (new);
				i = new->functionID-1;
				nParams = FUNCTION_nParameters(new->functionID);
				tmpFrom = Ini_GetDataArray (ini, sectionName, INI_TAG_WFM_functionParametersFrom, &dummy, VAL_DOUBLE);
				tmpTo =Ini_GetDataArray (ini, sectionName, INI_TAG_WFM_functionParametersTo, &dummy2,  VAL_DOUBLE);
				tmpStepType = Ini_GetDataArray (ini, sectionName, INI_TAG_WFM_functionParametersVary, &dummy3,VAL_INTEGER);
				if (dummy != nParams || dummy2 != nParams || dummy != nParams) {
					tprintf("error loading waveform %s\n",new->name);
					break;
				}
				for (j=0; j < nParams; j++) {
					new->functionParameters[i][j].from = tmpFrom[j];	
					new->functionParameters[i][j].to = tmpTo[j];	
					new->functionParameters[i][j].stepType = tmpStepType[j];	
				}
				for (j=0; j < nParams; j++) {
					if (tmpStepType[j] == WFM_STEP_TYPE_LIST) {
						sprintf(functionParameterListTag,INI_TAG_WFM_functionParametersList,j); 
						new->functionParameters[i][j].list = Ini_GetDataArray (ini, sectionName, functionParameterListTag, &(new->functionParameters[i][j].listSize), VAL_DOUBLE); 
					}
				}
				free(tmpFrom);
				free(tmpTo);
				free(tmpStepType);
				
			}
			Ini_Get_Int64 (ini, sectionName, INI_TAG_WFM_tScaleFrom, &new->durationFrom_50ns);
			Ini_Get_Int64 (ini, sectionName, INI_TAG_WFM_tScaleTo, &new->durationTo_50ns);
			Ini_GetInt (ini, sectionName, INI_TAG_WFM_tScaleVary, &new->durationVary);
			Ini_GetInt (ini, sectionName, INI_TAG_WFM_functionReverseTime, &new->functionReverseTime);

			break;
		case WFM_TYPE_ADDRESSING:
			Ini_GetInt (ini, sectionName, INI_TAG_WFM_addrLinkWfm, &new->addrLinkWfm);
			Ini_GetInt (ini, sectionName, INI_TAG_WFM_addrType, &new->addrType);
			Ini_GetDataArrayEx (ini, sectionName, INI_TAG_WFM_addrCalibration, new->addrCalibration, WFM_ADDR_N_CALIBRATION_PARAMETERS, VAL_DOUBLE);
			Ini_GetDataArrayEx (ini, sectionName, INI_TAG_WFM_addrCalibrationStepTo, new->addrCalibrationStepTo, WFM_ADDR_N_CALIBRATION_PARAMETERS, VAL_DOUBLE);
			Ini_GetDataArrayEx (ini, sectionName, INI_TAG_WFM_addrCalibrationStep, new->addrCalibrationStep, WFM_ADDR_N_CALIBRATION_PARAMETERS, VAL_INTEGER);
			if ((new->addrCalibration[WFM_ADDR_CAL_enableCal1] == 0) && (new->addrCalibration[WFM_ADDR_CAL_enableCal2] == 0)) {
				new->addrCalibration[WFM_ADDR_CAL_enableCal1] = 1;
				new->addrCalibration[WFM_ADDR_CAL_enableCal2] = 1;
			}

			Ini_GetPoint (ini, sectionName, INI_TAG_WFM_addrCenter, &new->addrCenter);
			Ini_GetStringCopy (ini, sectionName, INI_TAG_WFM_addrTestFilename, &new->addrTestFilename);
			Ini_GetStringCopy (ini, sectionName, INI_TAG_WFM_addrTestFilename2, &new->addrTestFilename2);
			Ini_Get_Int64 (ini, sectionName, INI_TAG_WFM_tScaleFrom, &new->durationFrom_50ns);
			Ini_GetInt (ini, sectionName, INI_TAG_WFM_addrEnablePhaseFeedback, &new->addrEnablePhaseFeedback);
			Ini_GetInt (ini, sectionName, INI_TAG_WFM_addrEnablePhaseFeedback2, &new->addrEnablePhaseFeedback2);
			Ini_GetInt (ini, sectionName, INI_TAG_WFM_addrCamera, &new->addrCamera);
			Ini_GetInt (ini, sectionName, INI_TAG_WFM_addrAddLastOffset, &new->addrAddLastOffset);
			Ini_GetInt (ini, sectionName, INI_TAG_WFM_addrAddLastOffsetNotAtFirst, &new->addrAddLastOffsetNotAtFirst);
			Ini_GetInt (ini, sectionName, INI_TAG_WFM_addrImageSize, &new->addrImageSize);
			switch (new->addrType) {
				case WFM_ADDR_TYPE_TRAJECTORY:
					Ini_GetDataArrayEx (ini, sectionName, INI_TAG_WFM_addrTrajParams, new->addrTrajectParams, WFM_ADDR_N_TRAJ_PARAMETERS, VAL_DOUBLE);
					break;
				case WFM_ADDR_TYPE_DMD:
					new->addrDMDimage = (t_DMDimage*) malloc (sizeof (t_DMDimage));          
					DMDIMAGE_init (new->addrDMDimage);					
					Ini_GetDMDImage (ini, sectionName, new->addrDMDimage);
					Ini_GetDataArrayEx (ini, sectionName, INI_TAG_WFM_addDisorder, new->addDisorder, WFM_ADDR_MATRIX_NUM, VAL_INTEGER); 
					Ini_GetDataArrayEx (ini, sectionName, INI_TAG_WFM_disorderSeed, new->disorderSeed, WFM_ADDR_MATRIX_NUM, VAL_UNSIGNED_INTEGER);
					Ini_GetDataArrayEx (ini, sectionName, INI_TAG_WFM_disorderTyp, new->disorderTyp, WFM_ADDR_MATRIX_NUM, VAL_INTEGER); 

					WFM_ADDR_allocateMatrix (new); 
					Ini_GetInt(ini, sectionName, INI_TAG_WFM_stepAddrMatrix, &new->stepAddrMatrix);
					Ini_GetInt(ini, sectionName, INI_TAG_WFM_addrMatricesPerRepetition, &new->addrMatricesPerRepetition); 
					Ini_GetDataArrayEx (ini, sectionName, INI_TAG_WFM_addrMatrix, new->addrMatrix,WFM_ADDR_MATRIXDIM*WFM_ADDR_MATRIX_NUM, VAL_UNSIGNED_INTEGER);
					break;
				case WFM_ADDR_TYPE_SINGLESITE:
	
					WFM_ADDR_allocateMatrix (new); 
					Ini_GetInt(ini, sectionName, INI_TAG_WFM_stepAddrMatrix, &new->stepAddrMatrix);
					Ini_GetInt(ini, sectionName, INI_TAG_WFM_addrMatricesPerRepetition, &new->addrMatricesPerRepetition);
					Ini_GetDataArrayEx (ini, sectionName, INI_TAG_WFM_addrMatrix, new->addrMatrix,WFM_ADDR_MATRIXDIM*WFM_ADDR_MATRIX_NUM, VAL_UNSIGNED_INTEGER);
					
					break;
			}
			break;
			
			
	}
			
	
	Ini_GetInt (ini, sectionName, INI_TAG_WFM_add, &new->add);
	
	return 0;
};



/******************************************************************************/
/*
/*   FUNCTIONS: Ini_GetPoint (...)
/*			    Ini_PutPoint (...)
/*
/*    Reads/writes data for a (swept) waveform point
/*    --  called by function "loadSequence(...), "saveSequence(...)", 
/*
/******************************************************************************/

void Ini_GetTPoint (IniText ini, char *sectionName, t_point *p, int nr)
{
    char tagName[50];
	
    if (p == NULL) return;
	POINT_init (p);
	sprintf (tagName, INI_TAG_POINT_varyTime, nr);
	Ini_GetInt (ini, sectionName, tagName, &p->varyTime);
	sprintf (tagName, INI_TAG_POINT_timeStart, nr);
	Ini_GetDouble (ini, sectionName, tagName, &p->timeStart_ns);
	sprintf (tagName, INI_TAG_POINT_timeIncrement, nr);
	Ini_GetDouble (ini, sectionName, tagName, &p->timeIncrement_ns);
	sprintf (tagName, INI_TAG_POINT_varyValue, nr);
	Ini_GetInt (ini, sectionName, tagName, &p->varyValue);
	sprintf (tagName, INI_TAG_POINT_valueStart, nr);
	Ini_GetDouble (ini, sectionName, tagName, &p->valueStart);
	sprintf (tagName, INI_TAG_POINT_valueIncrement, nr);
	Ini_GetDouble (ini, sectionName, tagName, &p->valueIncrement);
	sprintf (tagName, INI_TAG_POINT_stepRep, nr);
	Ini_GetInt (ini, sectionName, tagName, &p->stepRepetitions);
	
	
}


void Ini_PutTPoint (IniText ini, char *sectionName, t_point *p, int nr)
{
    char tagName[50];
    
    if (p == NULL) return;
	
	sprintf (tagName, INI_TAG_POINT_timeStart, nr);
	Ini_PutDouble (ini, sectionName, tagName, p->timeStart_ns);
	if ((p->varyTime != 0) || (p->timeIncrement_ns != 0.0)) {
		sprintf (tagName, INI_TAG_POINT_varyTime, nr);
		Ini_PutInt (ini, sectionName, tagName, p->varyTime);
		sprintf (tagName, INI_TAG_POINT_timeIncrement, nr);
		Ini_PutDouble (ini, sectionName, tagName, p->timeIncrement_ns);
	}
	sprintf (tagName, INI_TAG_POINT_valueStart, nr);
	Ini_PutDouble (ini, sectionName, tagName, p->valueStart);
	if ((p->varyValue != 0) || (p->valueIncrement != 0.0)) {
		sprintf (tagName, INI_TAG_POINT_varyValue, nr);
		Ini_PutInt (ini, sectionName, tagName, p->varyValue);
		sprintf (tagName, INI_TAG_POINT_valueIncrement, nr);
		Ini_PutDouble (ini, sectionName, tagName, p->valueIncrement);
	}
	if (p->stepRepetitions != 1) {
		sprintf (tagName, INI_TAG_POINT_stepRep, nr);
		Ini_PutInt (ini, sectionName, tagName, p->stepRepetitions);
	}
}




/******************************************************************************/
/*
/*   FUNCTIONS: Ini_GetTransferFunction (...)
/*			    Ini_PutTransferFunction (...)
/*
/*    Reads/writes data for a transfer function 
/*    in Section "[TransferFunction...]" from/to "ini-style"-file
/*    --  called by function "loadSequence(...), "saveSequence(...)", 
/*
/******************************************************************************/

void Ini_GetTransferFunction (IniText ini, t_sequence *seq, int nr)
{
    t_transferFunction *new;
    char sectionName[30];
    char tagName[30];
    int i;
	
	sprintf (sectionName, INI_SECTION_TRANSFERFUNCTION, nr);

    new = TRANSFERFUNCT_new (seq);
    TRANSFERFUNCT_init (new);
  
  
	Ini_GetStringIntoBuffer (ini, sectionName, INI_TAG_TRANSFERFUNCTION_name, new->name,
							 MAX_TRANSFERFUNCTNAME_LEN);
	Ini_GetInt (ini, sectionName, INI_TAG_TRANSFERFUNCTION_type,
			  	&new->type);
//	Ini_GetDouble (ini, sectionName, INI_TAG_TRANSFERFUNCTION_defaultVoltage,
//			       &new->defaultVoltage);
			  	
	Ini_GetInt (ini, sectionName, INI_TAG_TRANSFERFUNCTION_nParameters,
			  	&new->nParameters);
	Ini_GetDataArrayEx (ini, sectionName, INI_TAG_TRANSFERFUNCTION_parameters,
					    new->parameters, MAX_TRANSFERFUNCT_PARAMETERS, VAL_DOUBLE);
	for (i = 0; i < new->nParameters; i++) {
		if (new->parameterNames[i][0] != 0) {
			sprintf (tagName, INI_TAG_TRANSFERFUNCTION_parameterName, i);
			Ini_GetStringIntoBuffer (ini, sectionName, tagName, new->parameterNames[i], MAX_TRANSFERFUNCT_PARAMETER_NAME_LEN);
		}
	}
	
	Ini_GetStringIntoBuffer (ini, sectionName, INI_TAG_TRANSFERFUNCTION_xAxis, new->xAxis, MAX_TRANSFERFUNCT_AXIS_NAME_LEN);
	Ini_GetStringIntoBuffer (ini, sectionName, INI_TAG_TRANSFERFUNCTION_yAxis, new->yAxis, MAX_TRANSFERFUNCT_AXIS_NAME_LEN);
	

}


void Ini_PutTransferFunction (IniText ini, t_sequence *seq, int nr)
{
    t_transferFunction *new;
    char sectionName[30];
    char tagName[30];
    int i;
	
	sprintf (sectionName, INI_SECTION_TRANSFERFUNCTION, nr);

    new = TRANSFERFUNCT_ptr (seq, nr);
    if (new == NULL) return;
	Ini_PutString (ini, sectionName, INI_TAG_TRANSFERFUNCTION_name, new->name);
	
	
	Ini_PutInt (ini, sectionName, INI_TAG_TRANSFERFUNCTION_type, new->type);
//	Ini_PutDouble (ini, sectionName, INI_TAG_TRANSFERFUNCTION_defaultVoltage,
//			       new->defaultVoltage);
	Ini_PutInt (ini, sectionName, INI_TAG_TRANSFERFUNCTION_nParameters,
			  	new->nParameters);
	
	Ini_PutDataArray (ini, sectionName, INI_TAG_TRANSFERFUNCTION_parameters,
					  new->parameters, new->nParameters, VAL_DOUBLE);
	for (i = 0; i < new->nParameters; i++) {
		if (new->parameterNames[i][0] != 0) {
			sprintf (tagName, INI_TAG_TRANSFERFUNCTION_parameterName, i);
			Ini_PutString (ini, sectionName, tagName, new->parameterNames[i]);
		}
	}
	
	Ini_PutString (ini, sectionName, INI_TAG_TRANSFERFUNCTION_xAxis, new->xAxis);
	Ini_PutString (ini, sectionName, INI_TAG_TRANSFERFUNCTION_yAxis, new->yAxis);
}




/******************************************************************************/
/*
/*   FUNCTIONS: Ini_GetAnalogBlock (...)
/*			    Ini_PutAnalogBlock (...)
/*
/*    Reads/writes data for an ananlog block in 
/*    in Section "[AnalogBlock...]" from/to "ini-style"-file
/*    --  called by function "loadSequence(...), "saveSequence(...)", 
/*	  --  uses datatype "IniText" defined in Libaray "inifile.c"
/*
/******************************************************************************/

void Ini_GetAnalogBlock (IniText ini, t_digitalBlock *b, int analogBlockNr)
{
    char sectionName[30];
    char tagName[30];
    int i;
	
	sprintf (sectionName, INI_SECTION_ANALOGBLOCK, analogBlockNr);
    for (i=0; i < N_DAC_CHANNELS; i++) {
    	sprintf (tagName, INI_TAG_ANALOGBLOCK_Waveform, str_AnalogChannelNames(i));
        Ini_GetInt (ini, sectionName, tagName, &b->waveforms[i]);
    	sprintf (tagName, INI_TAG_ANALOGBLOCK_ConstVoltage, str_AnalogChannelNames(i));
        Ini_GetDouble (ini, sectionName, tagName, &b->constVoltages[i]);
    }
}

/*

void Ini_GetAnalogBlock (IniText ini, t_sequence *seq, int nr)
{
    t_analogBlock *new;
    char sectionName[30];
    char tagName[30];
    int i;
	
	sprintf (sectionName, INI_SECTION_ANALOGBLOCK, nr);

    new = ANALOGBLOCK_new (seq);
    ANALOGBLOCK_init (new);
  
	Ini_GetStringCopy (ini, sectionName, INI_TAG_WFM_Name, &new->name);
    for (i=0; i < N_AO_CHANNELS; i++) {
    	sprintf (tagName, INI_TAG_ANALOGBLOCK_Waveform, str_AnalogChannelNames(i));
        Ini_GetInt (ini, sectionName, tagName, &new->waveforms[i]);
    	sprintf (tagName, INI_TAG_ANALOGBLOCK_ConstVoltage, str_AnalogChannelNames(i));
        Ini_GetDouble (ini, sectionName, tagName, &new->constVoltages[i]);
    }
}

*/
/******************************************************************************/
/*
void Ini_PutAnalogBlock (IniText ini, t_sequence *seq, int nr)
{
    t_analogBlock *new;
    char sectionName[30];
    char tagName[30];
    int i;
	
	sprintf (sectionName, INI_SECTION_ANALOGBLOCK, nr);
    new = ANALOGBLOCK_ptr (seq, nr);
  
	Ini_PutString (ini, sectionName, INI_TAG_WFM_Name, new->name);
    for (i=0; i < N_AO_CHANNELS; i++) {
    	if (new->waveforms[i] != WFM_ID_UNCHANGED) {
	    	sprintf (tagName, INI_TAG_ANALOGBLOCK_Waveform, str_AnalogChannelNames(i));
	        Ini_PutInt (ini, sectionName, tagName, new->waveforms[i]);
	    }
    	if (new->waveforms[i] == WFM_ID_CONSTVOLTAGE) {
	    	sprintf (tagName, INI_TAG_ANALOGBLOCK_ConstVoltage, str_AnalogChannelNames(i));
	        Ini_PutDouble (ini, sectionName, tagName, new->constVoltages[i]);
	    }
    }
}
*/




/******************************************************************************/
/*
/*   FUNCTIONS: Ini_GetDigitalBlock (...)
/*			    Ini_PutDigitalBlock (...)
/*
/*    Reads/writes data for a digital block in 
/*    in Section "[DigitalBlock...]" from/to "ini-style"-file
/*    --  called by function "loadSequence(...), "saveSequence(...)", 
/*	  --  uses datatype "IniText" defined in Libaray "inifile.c"
/*
/******************************************************************************/

void Ini_GetDigitalBlock (IniText ini, t_sequence *seq, int nr)
{
    t_digitalBlock *new;
    char sectionName[30];
    char tagName[30];
    int i, n;
    
    
    int analogBlock = 0; // old version prior to 2.50 which had analog blocks
    				
	
	sprintf (sectionName, INI_SECTION_DIGITALBLOCK, nr);

    new = DIGITALBLOCK_new (seq);
    DIGITALBLOCK_init (new);
  
	Ini_GetStringIntoBuffer (ini, sectionName, INI_TAG_DIGITALBLOCK_Name, new->blockName, MAX_DIGITALBLOCK_NAME_LEN-1);
	Ini_GetStringIntoBuffer (ini, sectionName, INI_TAG_DIGITALBLOCK_groupName, new->groupName, MAX_DIGITALBLOCK_NAME_LEN-1);
    if (seq->programVersionCreatedWith < 2.70) {
	    // old format (prior to version 2.70)
	    for (i=0; i < seq->maxDigitalChannel; i++) {
	    	sprintf (tagName, INI_OLD_TAG_DIGITALBLOCK_Ch, str_ChannelNames(i));
	        Ini_GetInt (ini, sectionName, tagName, &new->channels[i]);
	    }
	}
	else {
		// new format
		Ini_GetDataArrayEx (ini, sectionName, INI_TAG_DIGITALBLOCK_Channels, new->channels, N_TOTAL_DIO_CHANNELS, 
							 VAL_INTEGER);
		Ini_GetDataArrayEx (ini, sectionName, INI_TAG_DIGITALBLOCK_alternate, new->alternate, N_TOTAL_DIO_CHANNELS, 
					        VAL_INTEGER);
		
	}

	Ini_GetInt (ini, sectionName, INI_TAG_DIGITALBLOCK_groupNr, &new->groupNr);
	Ini_GetInt (ini, sectionName, INI_TAG_DIGITALBLOCK_waitForExtTrigger, &new->waitForExtTrigger);
	Ini_GetInt (ini, sectionName, INI_TAG_DIGITALBLOCK_disable, &new->disable);
	Ini_GetInt (ini, sectionName, INI_TAG_DIGITALBLOCK_isGroupExpanded, &new->isGroupExpanded);
	Ini_GetInt (ini, sectionName, INI_TAG_DIGITALBLOCK_gpibCommandNr, &new->gpibCommandNr);

	
	Ini_GetUInt (ini, sectionName, INI_TAG_DIGITALBLOCK_Time,
			  &new->duration);
	
	// load sequence from old program version
	Ini_GetInt (ini, sectionName, INI_OLD_TAG_DIGITALBLOCK_AnalogBlock,
			  &analogBlock);
	Ini_GetInt (ini, sectionName, INI_TAG_DIGITALBLOCK_AnalogBlock,
			  &analogBlock);
	if (analogBlock != 0) {
		// Get AnalogBlock (from older program version)
		Ini_GetAnalogBlock (ini, new, analogBlock);
	}
	else {
		Ini_GetDataArrayEx_Comp (ini, sectionName, INI_TAG_DIGITALBLOCK_waveforms,
							new->waveforms, N_DAC_CHANNELS, VAL_INTEGER);
		Ini_GetDataArrayEx_Comp (ini, sectionName, INI_TAG_DIGITALBLOCK_constVoltages,
							new->constVoltages, N_DAC_CHANNELS, VAL_DOUBLE);
	}
	n = ListNumItems (seq->lWaveforms);
	for (i = 0; i < N_DAC_CHANNELS; i++) {
		if (new->waveforms[i] > n) 
			new->waveforms[i] = WFM_ID_UNCHANGED;
	}
	
	Ini_GetInt (ini, sectionName, INI_TAG_DIGITALBLOCK_BlockMode,
			  &new->blockMode);
	Ini_GetInt (ini, sectionName, INI_TAG_DIGITALBLOCK_VariableTime,
			  &new->variableTime);
	Ini_GetInt (ini, sectionName, INI_TAG_DIGITALBLOCK_IncrementTime,
			  &new->incrementTime);
	Ini_GetInt (ini, sectionName, INI_TAG_DIGITALBLOCK_VariableTimeRep,
			  &new->variableTimeExecuteRep);
	Ini_GetInt (ini, sectionName, INI_TAG_DIGITALBLOCK_VariableTimeRepOffset,
			  &new->variableTimeExecuteRepOffset);
	Ini_GetInt (ini, sectionName, INI_TAG_DIGITALBLOCK_VariableTimeStep,
			  &new->variableTimeStepRep);
	Ini_GetInt (ini, sectionName, INI_TAG_DIGITALBLOCK_VariableTimeModulo,
			  &new->variableTimeModulo);
	if (new->variableTimeModulo <= 1) new->variableTimeModulo = 9999;
			  
	Ini_GetInt (ini, sectionName, INI_TAG_DIGITALBLOCK_isAbsTimeReference,
			  &new->isAbsoluteTimeReference);
	
	Ini_GetInt (ini, sectionName, INI_TAG_DIGITALBLOCK_doTriggerLoop,
			  &new->doTriggerLoop);
	Ini_GetInt (ini, sectionName, INI_TAG_DIGITALBLOCK_doRecvTriggerLoop,
			  &new->doRecvTriggerLoop);
  
	Ini_GetDataArrayEx_Comp (ini, sectionName, INI_TAG_DIGITALBLOCK_varyPulse,
						new->varyPulse, N_TOTAL_DIO_CHANNELS, VAL_INTEGER);
	Ini_GetDataArrayEx_Comp (ini, sectionName, INI_TAG_DIGITALBLOCK_varyPulseStartIncr,
						new->varyPulseStartIncrement, N_TOTAL_DIO_CHANNELS, VAL_INTEGER);
	Ini_GetDataArrayEx_Comp (ini, sectionName, INI_TAG_DIGITALBLOCK_varyPulseStopIncr,
						new->varyPulseStopIncrement, N_TOTAL_DIO_CHANNELS, VAL_INTEGER);
	Ini_GetDataArrayEx_Comp (ini, sectionName, INI_TAG_DIGITALBLOCK_varyPulseStepRep,
						new->varyPulseStepRep, N_TOTAL_DIO_CHANNELS, VAL_INTEGER);
			
}
/******************************************************************************/





void Ini_PutDigitalBlock (IniText ini, t_sequence *seq, int nr)
{
    
    t_digitalBlock *new;
    char sectionName[30];
    //char tagName[30];
    //int i;
	
	sprintf (sectionName, INI_SECTION_DIGITALBLOCK, nr);
    new = DIGITALBLOCK_ptr (seq, nr);
  
	if (new->blockName[0] != 0) 
		Ini_PutString (ini, sectionName, INI_TAG_DIGITALBLOCK_Name, new->blockName);
	if (new->groupName[0] != 0) 
		Ini_PutString (ini, sectionName, INI_TAG_DIGITALBLOCK_groupName, new->groupName);
	Ini_PutInt0 (ini, sectionName, INI_TAG_DIGITALBLOCK_groupNr, new->groupNr);
	Ini_PutInt0 (ini, sectionName, INI_TAG_DIGITALBLOCK_waitForExtTrigger, new->waitForExtTrigger);
	Ini_PutInt0 (ini, sectionName, INI_TAG_DIGITALBLOCK_disable, new->disable);
	Ini_PutInt0 (ini, sectionName, INI_TAG_DIGITALBLOCK_isGroupExpanded, new->isGroupExpanded);
	Ini_PutInt0 (ini, sectionName, INI_TAG_DIGITALBLOCK_gpibCommandNr, new->gpibCommandNr);
	

	Ini_PutUInt (ini, sectionName, INI_TAG_DIGITALBLOCK_Time,
			  new->duration);

	if (new->blockMode != BMODE_Standard) 
		Ini_PutInt (ini, sectionName, INI_TAG_DIGITALBLOCK_BlockMode,
			  new->blockMode);
			  
	if (new->variableTime) {
		Ini_PutInt (ini, sectionName, INI_TAG_DIGITALBLOCK_VariableTime,
					  new->variableTime);
		if (new->incrementTime != 0.0) 
			Ini_PutInt (ini, sectionName, INI_TAG_DIGITALBLOCK_IncrementTime,
					  new->incrementTime);
		if (new->variableTimeExecuteRep != 1)
			Ini_PutInt (ini, sectionName, INI_TAG_DIGITALBLOCK_VariableTimeRep,
					  new->variableTimeExecuteRep);
		Ini_PutInt0 (ini, sectionName, INI_TAG_DIGITALBLOCK_VariableTimeRepOffset, new->variableTimeExecuteRepOffset);
		if (new->variableTimeStepRep != 1)
			Ini_PutInt (ini, sectionName, INI_TAG_DIGITALBLOCK_VariableTimeStep,
					  new->variableTimeStepRep);
		if (new->variableTimeModulo != 0)
			Ini_PutInt (ini, sectionName, INI_TAG_DIGITALBLOCK_VariableTimeModulo,
					  new->variableTimeModulo);
	}
	
	Ini_PutInt (ini, sectionName, INI_TAG_DIGITALBLOCK_doTriggerLoop,
			  new->doTriggerLoop);
	Ini_PutInt (ini, sectionName, INI_TAG_DIGITALBLOCK_doRecvTriggerLoop,
			  new->doRecvTriggerLoop);
	
    // put channels
	Ini_PutDataArray (ini, sectionName, INI_TAG_DIGITALBLOCK_Channels, new->channels, N_TOTAL_DIO_CHANNELS, 
					  VAL_INTEGER);
	Ini_PutDataArray (ini, sectionName, INI_TAG_DIGITALBLOCK_alternate, new->alternate, N_TOTAL_DIO_CHANNELS, 
					  VAL_INTEGER);
	// put channels
    if (new->isAbsoluteTimeReference) 
		Ini_PutInt (ini, sectionName, INI_TAG_DIGITALBLOCK_isAbsTimeReference,
		   new->isAbsoluteTimeReference);

	if (DIGITALBLOCK_hasWaveform (new)) {
		Ini_PutDataArray_Comp (ini, sectionName, INI_TAG_DIGITALBLOCK_waveforms,
							new->waveforms, N_DAC_CHANNELS, VAL_INTEGER);
	}
	
	if (DIGITALBLOCK_hasConstVoltage (new)) {
		Ini_PutDataArray_Comp (ini, sectionName, INI_TAG_DIGITALBLOCK_constVoltages,
							new->constVoltages, N_DAC_CHANNELS, VAL_DOUBLE);
	}
	if (isArrayNotEqual_int (new->varyPulse, N_TOTAL_DIO_CHANNELS, 0)) {
		Ini_PutDataArray_Comp (ini, sectionName, INI_TAG_DIGITALBLOCK_varyPulse,
							new->varyPulse, N_TOTAL_DIO_CHANNELS, VAL_INTEGER);
	}
	if (isArrayNotEqual_int (new->varyPulseStartIncrement, N_TOTAL_DIO_CHANNELS, 0)) {
		Ini_PutDataArray_Comp (ini, sectionName, INI_TAG_DIGITALBLOCK_varyPulseStartIncr,
							new->varyPulseStartIncrement, N_TOTAL_DIO_CHANNELS, VAL_INTEGER);
	}
	if (isArrayNotEqual_int (new->varyPulseStopIncrement, N_TOTAL_DIO_CHANNELS, 0)) {
		Ini_PutDataArray_Comp (ini, sectionName, INI_TAG_DIGITALBLOCK_varyPulseStopIncr,
							new->varyPulseStopIncrement, N_TOTAL_DIO_CHANNELS, VAL_INTEGER);
	}
	if (isArrayNotEqual_int (new->varyPulseStepRep, N_TOTAL_DIO_CHANNELS, 1)) {
		Ini_PutDataArray_Comp (ini, sectionName, INI_TAG_DIGITALBLOCK_varyPulseStepRep,
						  new->varyPulseStepRep, N_TOTAL_DIO_CHANNELS, VAL_INTEGER);
	}
}






/******************************************************************************/
/*
/*   FUNCTIONS: Ini_GetGpibCommand (...)
/*			    Ini_PutGpibCommand (...)
/*
/*    Reads/writes GpibCommands 
/*    in Section "[GPIBCommands...]" from/to "ini-style"-file
/*    --  called by function "loadSequence(...), "saveSequence(...)", 
/*	  --  uses datatype "IniText" defined in Libaray "inifile.c"
/*
/******************************************************************************/




void Ini_GetGpibCommand (IniText ini, t_sequence *seq, int nr)
{
    t_gpibCommand *new;
    int i;
    char sectionName[30];
    char tagName[50];

	sprintf (sectionName, INI_SECTION_GPIBCOMMAND, nr);
    new = GPIBCOMMAND_new (seq);
    GPIBCOMMAND_init (new, GPIB_COMMANDTYPE_NONE);

	Ini_GetStringIntoBuffer (ini, sectionName, 
			INI_TAG_GPIBCOMMAND_name, new->name, MAX_GPIBCOMMANDNAME_LEN);
	Ini_GetInt (ini, sectionName, INI_TAG_GPIBCOMMAND_gpibAddress,
				 &new->gpibAddress);
	Ini_GetInt (ini, sectionName, INI_TAG_GPIBCOMMAND_commandType, 
				 &new->commandType);
	Ini_GetInt (ini, sectionName, INI_TAG_GPIBCOMMAND_noGPIBdevice, 
				 &new->noGPIBdevice);
	if (new->noGPIBdevice){
		Ini_GetUInt (ini, sectionName, INI_TAG_GPIBCOMMAND_extDeviceID, 
				 &new->extDeviceID);
		Ini_GetUInt (ini, sectionName, INI_TAG_GPIBCOMMAND_extDeviceChannel, 
				 &new->extDeviceChannel);
	}
	Ini_GetInt (ini, sectionName, INI_TAG_GPIBCOMMAND_transmitBeforeFirst,
				&new->transmitBeforeFirstRepetition);
	Ini_GetInt (ini, sectionName, INI_TAG_GPIBCOMMAND_transmitBeforeEvery,
				&new->transmitBeforeEveryRepetition);
	Ini_GetInt (ini, sectionName, INI_TAG_GPIBCOMMAND_transmitAfterEvery,
				&new->transmitAfterEveryRepetition);
	Ini_GetInt (ini, sectionName, INI_TAG_GPIBCOMMAND_transmitAfterLast,
				&new->transmitAfterLastRepetition);
	Ini_GetInt (ini, sectionName, INI_TAG_GPIBCOMMAND_transmitAfterStartOfSequence,
				&new->transmitAfterStartOfSequence);
	Ini_GetDouble (ini, sectionName, INI_TAG_GPIBCOMMAND_maximumDuration,
				&new->maximumDuration_ms);
	
	 

	switch (new->commandType) {
		case GPIB_COMMANDTYPE_ARBWFM:
			Ini_GetInt (ini, sectionName, INI_TAG_GPIBCOMMAND_waveform,
						 &new->waveform );
			Ini_GetInt (ini, sectionName, INI_TAG_GPIBCOMMAND_channel,
						 &new->channel);
			Ini_GetUInt (ini, sectionName,  INI_TAG_GPIBCOMMAND_nPoints, 
						&new->nPoints);
			Ini_GetDouble (ini, sectionName,  INI_TAG_GPIBCOMMAND_duration, 
						&new->duration_us);
			Ini_GetInt (ini, sectionName,  INI_TAG_GPIBCOMMAND_askDeviceForErrors, 
						&new->askDeviceForErrors);
			Ini_GetInt (ini, sectionName,  INI_TAG_GPIBCOMMAND_wfmAlwaysMaxPts, 
						&new->wfmAlwaysMaxPts);
			Ini_GetInt (ini, sectionName,  INI_TAG_GPIBCOMMAND_wfmTransferMode, 
						&new->wfmTransferMode);
			
			break;
		case GPIB_COMMANDTYPE_FREQ:
			Ini_GetInt (ini, sectionName,  INI_TAG_GPIBCOMMAND_enableStep, 
						&new->enableStep);
			Ini_GetDouble (ini, sectionName,  INI_TAG_GPIBCOMMAND_startFreq, 
						&new->startFreq);
			Ini_GetDouble (ini, sectionName,  INI_TAG_GPIBCOMMAND_stopFreq, 
						&new->stopFreq);
			Ini_GetInt (ini, sectionName,  INI_TAG_GPIBCOMMAND_enterSpan, 
						&new->enterSpan);
			Ini_GetInt (ini, sectionName,  INI_TAG_GPIBCOMMAND_fetchFreq, 
						&new->fetchFreq);
			Ini_GetInt (ini, sectionName,  INI_TAG_GPIBCOMMAND_addFreqOffset, 
						&new->addFreqOffset);
			Ini_GetDouble (ini, sectionName,  INI_TAG_GPIBCOMMAND_freqOffsetMHz, 
						&new->freqOffsetMHz);
			Ini_GetInt (ini, sectionName,  INI_TAG_GPIBCOMMAND_nStepRepetition, 
						&new->nStepRepetition);
			Ini_GetInt (ini, sectionName,  INI_TAG_GPIBCOMMAND_stepTrigger, 
						&new->stepTrigger);
			Ini_GetInt (ini, sectionName,  INI_TAG_GPIBCOMMAND_divideFreq, 
						&new->divideFreq);
			Ini_GetDouble (ini, sectionName,  INI_TAG_GPIBCOMMAND_startMultiply, 
						&new->startMultiply);
			Ini_GetDouble (ini, sectionName,  INI_TAG_GPIBCOMMAND_stopMultiply, 
						&new->stopMultiply);
			Ini_GetDouble (ini, sectionName,  INI_TAG_GPIBCOMMAND_centerMultiply, 
						&new->centerMultiply);
			Ini_GetDouble (ini, sectionName,  INI_TAG_GPIBCOMMAND_spanMultiply, 
						&new->spanMultiply);
			Ini_GetInt (ini, sectionName,  INI_TAG_GPIBCOMMAND_rfOn, 
						&new->rfOn);
			Ini_GetInt (ini, sectionName,  INI_TAG_GPIBCOMMAND_pulseModeEnable, 
						&new->pulseModeEnable);
			Ini_GetDouble (ini, sectionName,  INI_TAG_GPIBCOMMAND_outputPower, 
						&new->outputPower);
			Ini_GetInt (ini, sectionName,  INI_TAG_GPIBCOMMAND_enableFM , 
						&new->enableFM );
			Ini_GetDouble (ini, sectionName,  INI_TAG_GPIBCOMMAND_freqDeviation , 
							&new->freqDeviation);
			Ini_GetDouble (ini, sectionName,  INI_TAG_GPIBCOMMAND_freqDeviationMultiply , 
							&new->freqDeviationMultiply);
			Ini_GetInt (ini, sectionName,  INI_TAG_GPIBCOMMAND_enableAM, 
						&new->enableAM );
			Ini_GetDouble (ini, sectionName,  INI_TAG_GPIBCOMMAND_AMdepthPercent, 
							&new->AMdepthPercent);
			for (i = 0; i < N_FEEDBACK_CHANNELS; i++) {
				sprintf (tagName, INI_TAG_GPIBCOMMAND_enableFeedback, i);
				Ini_GetInt (ini, sectionName,  tagName, &new->enableFeedback[i]);
				sprintf (tagName, INI_TAG_GPIBCOMMAND_feedbackAnalogInput, i);
				Ini_GetStringIntoBuffer (ini, sectionName, tagName,
										 new->feedbackAnalogInput[i],
										 MAX_GPIB_ANALOGINPUTNAME_LEN);
				sprintf (tagName, INI_TAG_GPIBCOMMAND_feedbackFreqDeviation, i);
				Ini_GetDouble (ini, sectionName, tagName, &new->feedbackFreqDeviation[i]);
				sprintf (tagName, INI_TAG_GPIBCOMMAND_feedbackOffset, i);
				Ini_GetDouble (ini, sectionName,  tagName, &new->feedbackOffset[i]);
			}			
			break;
		case GPIB_COMMANDTYPE_STRINGS:
			for (i = 0; i < GPIB_NCOMMANDSTRINGS; i++) {
		    	sprintf (tagName, INI_TAG_GPIBCOMMAND_commandStringSend, i);
				Ini_GetInt (ini, sectionName, tagName, &new->commandStringSend[i]);
		    	sprintf (tagName, INI_TAG_GPIBCOMMAND_commandString, i);
				Ini_GetStringCopy (ini, sectionName, tagName, &new->commandString[i]);
		    	sprintf (tagName, INI_TAG_GPIBCOMMAND_commandStringReceive, i);
				Ini_GetInt (ini, sectionName, tagName, &new->commandStringReceive[i]);
		    	sprintf (tagName, INI_TAG_GPIBCOMMAND_commandStringErrorCheck, i);
				Ini_GetInt (ini, sectionName, tagName, &new->commandStringErrorCheck[i]);
			}
		    break;
		case GPIB_COMMANDTYPE_POWERSUPPLY:
			Ini_GetInt (ini, sectionName, INI_TAG_GPIBCOMMAND_channel,
					    &new->channel);
			Ini_GetInt (ini, sectionName,  INI_TAG_GPIBCOMMAND_nStepRepetition, 
						&new->nStepRepetition);
			Ini_GetInt (ini, sectionName,  INI_TAG_GPIBCOMMAND_stepVoltage, 
						&new->stepVoltage);
			Ini_GetDouble (ini, sectionName,  INI_TAG_GPIBCOMMAND_startVoltage, 
						   &new->startVoltage);
			Ini_GetDouble (ini, sectionName,  INI_TAG_GPIBCOMMAND_stopVoltage, 
						   &new->stopVoltage);
			Ini_GetInt (ini, sectionName,  INI_TAG_GPIBCOMMAND_stepCurrent, 
						&new->stepCurrent);
			Ini_GetDouble (ini, sectionName,  INI_TAG_GPIBCOMMAND_startCurrent, 
						   &new->startCurrent);
			Ini_GetDouble (ini, sectionName,  INI_TAG_GPIBCOMMAND_stopCurrent, 
						   &new->stopCurrent);
			Ini_GetInt (ini, sectionName,  INI_TAG_GPIBCOMMAND_waitUntilStabilized, 
						   &new->waitUntilStabilized);
			break;
		case GPIB_COMMANDTYPE_PULSES:
			Ini_GetDataArrayEx (ini, sectionName, INI_TAG_GPIBCOMMAND_startPulseDelay,
					  &new->startPulseDelay, GPIB_COMMAND_PULSES_CHANNELS, VAL_DOUBLE);
			Ini_GetDataArrayEx (ini, sectionName, INI_TAG_GPIBCOMMAND_stopPulseDelay,
					  &new->stopPulseDelay, GPIB_COMMAND_PULSES_CHANNELS, VAL_DOUBLE);
			Ini_GetDataArrayEx (ini, sectionName, INI_TAG_GPIBCOMMAND_stepPulseDelay,
					  &new->stepPulseDelay, GPIB_COMMAND_PULSES_CHANNELS, VAL_INTEGER);
			Ini_GetDataArrayEx (ini, sectionName, INI_TAG_GPIBCOMMAND_startPulseLength,
					  &new->startPulseLength, GPIB_COMMAND_PULSES_CHANNELS, VAL_DOUBLE);
			Ini_GetDataArrayEx (ini, sectionName, INI_TAG_GPIBCOMMAND_stopPulseLength,
					  &new->stopPulseLength, GPIB_COMMAND_PULSES_CHANNELS, VAL_DOUBLE);
			Ini_GetDataArrayEx (ini, sectionName, INI_TAG_GPIBCOMMAND_stepPulseLength,
					  &new->stepPulseLength, GPIB_COMMAND_PULSES_CHANNELS, VAL_INTEGER);			
			break;
	}

}

/******************************************************************************/
    

void Ini_PutGpibCommand (IniText ini, t_sequence *seq, int nr)
{
    t_gpibCommand *new;
    int i;
    char sectionName[30];
    char tagName[40];
	
	sprintf (sectionName,  INI_SECTION_GPIBCOMMAND, nr);
    new = GPIBCOMMAND_ptr (seq, nr);

	Ini_PutString (ini, sectionName, INI_TAG_GPIBCOMMAND_name, new->name);
	Ini_PutInt (ini, sectionName, INI_TAG_GPIBCOMMAND_gpibAddress,
				 new->gpibAddress);
	Ini_PutInt (ini, sectionName, INI_TAG_GPIBCOMMAND_commandType, 
				 new->commandType);
	Ini_PutInt (ini, sectionName, INI_TAG_GPIBCOMMAND_noGPIBdevice, 
				 new->noGPIBdevice);
	if (new->noGPIBdevice) {
		Ini_PutUInt (ini, sectionName, INI_TAG_GPIBCOMMAND_extDeviceID, 
				 new->extDeviceID);
		Ini_PutUInt (ini, sectionName, INI_TAG_GPIBCOMMAND_extDeviceChannel, 
				 new->extDeviceChannel);
	}
	Ini_PutInt0 (ini, sectionName, INI_TAG_GPIBCOMMAND_transmitBeforeFirst,
				new->transmitBeforeFirstRepetition);
	Ini_PutInt0 (ini, sectionName, INI_TAG_GPIBCOMMAND_transmitBeforeEvery,
				new->transmitBeforeEveryRepetition);
	Ini_PutInt0 (ini, sectionName, INI_TAG_GPIBCOMMAND_transmitAfterEvery,
				new->transmitAfterEveryRepetition);
	Ini_PutInt0 (ini, sectionName, INI_TAG_GPIBCOMMAND_transmitAfterLast,
				new->transmitAfterLastRepetition);
	Ini_PutInt0 (ini, sectionName, INI_TAG_GPIBCOMMAND_transmitAfterStartOfSequence,
				new->transmitAfterStartOfSequence);
	Ini_PutDouble (ini, sectionName, INI_TAG_GPIBCOMMAND_maximumDuration,
				new->maximumDuration_ms);

	
	switch (new->commandType) {
		case GPIB_COMMANDTYPE_ARBWFM:
			Ini_PutInt (ini, sectionName, INI_TAG_GPIBCOMMAND_waveform,
						 new->waveform );
			Ini_PutInt (ini, sectionName, INI_TAG_GPIBCOMMAND_channel,
						 new->channel);
//			Ini_PutUInt (ini, sectionName,  INI_TAG_GPIBCOMMAND_timebase, 
//						 new->timebase);
//			Ini_PutUInt (ini, sectionName,  INI_TAG_GPIBCOMMAND_timebaseDiv, 
//						 new->timebaseDiv);
			Ini_PutUInt (ini, sectionName,  INI_TAG_GPIBCOMMAND_nPoints, 
						new->nPoints);
			Ini_PutDouble (ini, sectionName,  INI_TAG_GPIBCOMMAND_duration, 
						new->duration_us);
			Ini_PutInt0 (ini, sectionName,  INI_TAG_GPIBCOMMAND_askDeviceForErrors, 
						new->askDeviceForErrors);
			Ini_PutInt (ini, sectionName,  INI_TAG_GPIBCOMMAND_wfmAlwaysMaxPts, 
						new->wfmAlwaysMaxPts);
			Ini_PutInt (ini, sectionName,  INI_TAG_GPIBCOMMAND_wfmTransferMode, 
						new->wfmTransferMode);
			break;
		case GPIB_COMMANDTYPE_FREQ:
			Ini_PutInt (ini, sectionName,  INI_TAG_GPIBCOMMAND_enableStep, 
						new->enableStep);
			Ini_PutDouble (ini, sectionName,  INI_TAG_GPIBCOMMAND_startFreq, 
						new->startFreq);
			Ini_PutDouble (ini, sectionName,  INI_TAG_GPIBCOMMAND_stopFreq, 
						new->stopFreq);
			Ini_PutInt (ini, sectionName,  INI_TAG_GPIBCOMMAND_enterSpan, 
						new->enterSpan);
			Ini_PutInt0 (ini, sectionName,  INI_TAG_GPIBCOMMAND_fetchFreq, 
						new->fetchFreq);
			Ini_PutInt0 (ini, sectionName,  INI_TAG_GPIBCOMMAND_addFreqOffset, 
						new->addFreqOffset);
			Ini_PutDouble (ini, sectionName,  INI_TAG_GPIBCOMMAND_freqOffsetMHz, 
						new->freqOffsetMHz);
			Ini_PutInt (ini, sectionName,  INI_TAG_GPIBCOMMAND_nStepRepetition, 
						new->nStepRepetition);
			Ini_PutInt (ini, sectionName,  INI_TAG_GPIBCOMMAND_stepTrigger, 
						new->stepTrigger);
			Ini_PutInt (ini, sectionName,  INI_TAG_GPIBCOMMAND_divideFreq, 
						new->divideFreq);
			Ini_PutDouble (ini, sectionName,  INI_TAG_GPIBCOMMAND_startMultiply, 
						new->startMultiply);
			Ini_PutDouble (ini, sectionName,  INI_TAG_GPIBCOMMAND_stopMultiply, 
						new->stopMultiply);
			Ini_PutDouble (ini, sectionName,  INI_TAG_GPIBCOMMAND_centerMultiply, 
						new->centerMultiply);
			Ini_PutDouble (ini, sectionName,  INI_TAG_GPIBCOMMAND_spanMultiply, 
						new->spanMultiply);
			Ini_PutInt (ini, sectionName,  INI_TAG_GPIBCOMMAND_rfOn, 
						new->rfOn);
			Ini_PutInt0 (ini, sectionName,  INI_TAG_GPIBCOMMAND_pulseModeEnable, 
						new->pulseModeEnable);
			Ini_PutDouble (ini, sectionName,  INI_TAG_GPIBCOMMAND_outputPower, 
						new->outputPower);
			Ini_PutInt0 (ini, sectionName,  INI_TAG_GPIBCOMMAND_enableFM , 
						new->enableFM );
			Ini_PutDouble (ini, sectionName,  INI_TAG_GPIBCOMMAND_freqDeviation , 
							new->freqDeviation);
			Ini_PutDouble (ini, sectionName,  INI_TAG_GPIBCOMMAND_freqDeviationMultiply , 
							new->freqDeviationMultiply);
			Ini_PutInt0 (ini, sectionName,  INI_TAG_GPIBCOMMAND_enableAM, 
						new->enableAM );
			if (new->AMdepthPercent != 100.0) {
				Ini_PutDouble (ini, sectionName,  INI_TAG_GPIBCOMMAND_AMdepthPercent, 
							  new->AMdepthPercent);
			}
			for (i = 0; i < N_FEEDBACK_CHANNELS; i++) {
				sprintf (tagName, INI_TAG_GPIBCOMMAND_enableFeedback, i);
				Ini_PutInt0 (ini, sectionName,  tagName, new->enableFeedback[i]);
				sprintf (tagName, INI_TAG_GPIBCOMMAND_feedbackAnalogInput, i);
				Ini_PutString0 (ini, sectionName,  tagName,  new->feedbackAnalogInput[i]);
				sprintf (tagName, INI_TAG_GPIBCOMMAND_feedbackFreqDeviation, i);
				Ini_PutDouble0 (ini, sectionName,  tagName, new->feedbackFreqDeviation[i]);
				sprintf (tagName, INI_TAG_GPIBCOMMAND_feedbackOffset, i);
				Ini_PutDouble0 (ini, sectionName,  tagName, new->feedbackOffset[i]);
			}			
			break;
		case GPIB_COMMANDTYPE_STRINGS:
			for (i = 0; i < GPIB_NCOMMANDSTRINGS; i++) {
				if (new->commandString[i] != NULL) {
					if (new->commandStringSend[i]) {
				    	sprintf (tagName, INI_TAG_GPIBCOMMAND_commandStringSend, i);
						Ini_PutInt (ini, sectionName, tagName, new->commandStringSend[i]);
					}
					if (strlen (new->commandString[i]) > 0) {
				    	sprintf (tagName, INI_TAG_GPIBCOMMAND_commandString, i);
						Ini_PutString (ini, sectionName, tagName, new->commandString[i]);
					}
					if (new->commandStringReceive[i]) {
				    	sprintf (tagName, INI_TAG_GPIBCOMMAND_commandStringReceive, i);
						Ini_PutInt (ini, sectionName, tagName, new->commandStringReceive[i]);
					}
					if (new->commandStringErrorCheck[i]) {
				    	sprintf (tagName, INI_TAG_GPIBCOMMAND_commandStringErrorCheck, i);
						Ini_PutInt (ini, sectionName, tagName, new->commandStringErrorCheck[i]);
					}
				}
			}
		    break;
		case GPIB_COMMANDTYPE_POWERSUPPLY:
			Ini_PutInt (ini, sectionName, INI_TAG_GPIBCOMMAND_channel,
					    new->channel);
			Ini_PutInt (ini, sectionName,  INI_TAG_GPIBCOMMAND_nStepRepetition, 
						new->nStepRepetition);
			Ini_PutDouble (ini, sectionName,  INI_TAG_GPIBCOMMAND_startVoltage, 
						   new->startVoltage);
			if (new->stepVoltage || (new->stopVoltage != 0)) {
				Ini_PutInt (ini, sectionName,  INI_TAG_GPIBCOMMAND_stepVoltage, 
							new->stepVoltage);
				Ini_PutDouble (ini, sectionName,  INI_TAG_GPIBCOMMAND_stopVoltage, 
							   new->stopVoltage);
			}
			Ini_PutDouble (ini, sectionName,  INI_TAG_GPIBCOMMAND_startCurrent, 
						   new->startCurrent);
			if (new->stepCurrent || (new->stopCurrent != 0)) {
				Ini_PutInt (ini, sectionName,  INI_TAG_GPIBCOMMAND_stepCurrent, 
							new->stepCurrent);
				Ini_PutDouble (ini, sectionName,  INI_TAG_GPIBCOMMAND_stopCurrent, 
							   new->stopCurrent);
			}
			Ini_PutInt0 (ini, sectionName,  INI_TAG_GPIBCOMMAND_waitUntilStabilized, 
						new->waitUntilStabilized);
			break;
		case GPIB_COMMANDTYPE_PULSES:
			Ini_PutDataArray (ini, sectionName, INI_TAG_GPIBCOMMAND_startPulseDelay,
					  new->startPulseDelay, GPIB_COMMAND_PULSES_CHANNELS, VAL_DOUBLE);
			Ini_PutDataArray (ini, sectionName, INI_TAG_GPIBCOMMAND_stopPulseDelay,
					  new->stopPulseDelay, GPIB_COMMAND_PULSES_CHANNELS, VAL_DOUBLE);
			Ini_PutDataArray (ini, sectionName, INI_TAG_GPIBCOMMAND_stepPulseDelay,
					  new->stepPulseDelay, GPIB_COMMAND_PULSES_CHANNELS, VAL_INTEGER);
			Ini_PutDataArray (ini, sectionName, INI_TAG_GPIBCOMMAND_startPulseLength,
					  new->startPulseLength, GPIB_COMMAND_PULSES_CHANNELS, VAL_DOUBLE);
			Ini_PutDataArray (ini, sectionName, INI_TAG_GPIBCOMMAND_stopPulseLength,
					  new->stopPulseLength, GPIB_COMMAND_PULSES_CHANNELS, VAL_DOUBLE);
			Ini_PutDataArray (ini, sectionName, INI_TAG_GPIBCOMMAND_stepPulseLength,
					  new->stepPulseLength, GPIB_COMMAND_PULSES_CHANNELS, VAL_INTEGER);
			break;
		    
	}
	
}






void Ini_PutTableConfig (IniText ini, t_tableConfig *c, int nr)
{
    
    char sectionName[30];
	
	sprintf (sectionName, INI_SECTION_TABLECONFIG, nr);
  
	Ini_PutString (ini, sectionName, INI_TAG_TABLECONFIG_name, c->name);

	Ini_PutDataArray (ini, sectionName, INI_TAG_TABLECONFIG_DIOvisible,
							c->DIOvisible, N_TOTAL_DIO_CHANNELS, VAL_INTEGER);
	Ini_PutDataArray (ini, sectionName, INI_TAG_TABLECONFIG_AOvisible,
							c->AOvisible, N_DAC_CHANNELS, VAL_INTEGER);
}





void Ini_GetTableConfig (IniText ini, t_tableConfig *c, int nr)
{
    
    char sectionName[30];
	
	sprintf (sectionName, INI_SECTION_TABLECONFIG, nr);
  
	Ini_GetStringIntoBuffer (ini, sectionName, INI_TAG_TABLECONFIG_name, c->name, TABLECONFIG_NAME_LEN);

	Ini_GetDataArrayEx (ini, sectionName, INI_TAG_TABLECONFIG_DIOvisible,
							c->DIOvisible, N_TOTAL_DIO_CHANNELS, VAL_INTEGER);
	Ini_GetDataArrayEx (ini, sectionName, INI_TAG_TABLECONFIG_AOvisible,
							c->AOvisible, N_DAC_CHANNELS, VAL_INTEGER);

}




void Ini_PutCCDSettings (IniText ini, t_sequence *seq, int nr, int iniFileForCamera)
{
    char sectionName[30];
	t_ccdSettings *c;
	int nPics;
	
	sprintf (sectionName, INI_SECTION_CCDSETTINGS, nr);
	ListGetItem (seq->lCCDSettings, &c, nr);
	
	if (!iniFileForCamera) Ini_PutInt0 (ini, sectionName, INI_TAG_CCDSETTINGS_enableCCD, c->enableCCD);
	Ini_PutInt0 (ini, sectionName, INI_TAG_CCDSETTINGS_serialNo, c->serialNo);
	if (!iniFileForCamera) Ini_PutString0 (ini, sectionName, INI_TAG_CCDSETTINGS_description, c->description);
	Ini_PutInt (ini, sectionName, INI_TAG_CCDSETTINGS_cameraType, c->cameraType);
	
	Ini_PutInt (ini, sectionName, INI_TAG_CCDSETTINGS_shutter, c->shutter);
	Ini_PutInt (ini, sectionName, INI_TAG_CCDSETTINGS_cooler, c->cooler);
	Ini_PutInt (ini, sectionName, INI_TAG_CCDSETTINGS_fanMode, c->fanMode);
	Ini_PutInt (ini, sectionName, INI_TAG_CCDSETTINGS_temperature, c->temperature);
	Ini_PutInt (ini, sectionName, INI_TAG_CCDSETTINGS_acquisition, 
				c->acquisition);
	Ini_PutInt (ini, sectionName, INI_TAG_CCDSETTINGS_preamplifierGain, c->preamplifierGain);
	Ini_PutInt (ini, sectionName, INI_TAG_CCDSETTINGS_shiftSpeed, c->shiftSpeed);
	Ini_PutInt (ini, sectionName, INI_TAG_CCDSETTINGS_readoutRate, c->readoutRate);
	
	CCD_getNumberOfPicturesPerRepetition (c, &nPics, 0);
	Ini_PutInt (ini, sectionName, INI_TAG_CCDSETTINGS_picturesPerRepetition, nPics);
	if (iniFileForCamera) {
		CCD_getNumberOfPicturesPerRepetition (c, &nPics, 0);
		Ini_PutInt (ini, sectionName, INI_TAG_CCDSETTINGS_picturesPerRepetition, nPics);
		Ini_PutString0 (ini, sectionName, INI_TAG_CCDSETTINGS_dataPath, c->dataPath);   
		if (seq->isOfTypeIdleSeq) { 
			Ini_PutInt(ini, sectionName, INI_TAG_CCDSETTINGS_fromIdleSequence, 1);    
			Ini_PutString(ini, sectionName, INI_TAG_CCDSETTINGS_idleSequenceDataPath, config->idleSequenceDataPath);
			Ini_PutInt (ini, sectionName, INI_TAG_CCDSETTINGS_seqNr, 0);
			tprintf("Writing ini file for camera for idle sequence\n");
		} else {
			Ini_PutString0 (ini, sectionName, INI_TAG_CCDSETTINGS_dataPath, c->dataPath);  
			Ini_PutInt (ini, sectionName, INI_TAG_CCDSETTINGS_seqNr, seq->startNr);
		}
		Ini_PutString (ini, sectionName, INI_TAG_CCDSETTINGS_date, dateStr('.'));
		Ini_PutString (ini, sectionName, INI_TAG_CCDSETTINGS_time, timeStr());
		Ini_PutInt (ini, sectionName, INI_TAG_CCDSETTINGS_daytimeInSeconds, daytimeInSeconds());
		Ini_PutInt (ini, sectionName, INI_TAG_CCDSETTINGS_nRepetitions, c->separateFilePerRepetition ? 1 : seq->nRepetitions);
		Ini_PutInt (ini, sectionName, INI_TAG_CCDSETTINGS_nCycles, c->separateFilePerRepetition ? seq->nRepetitions : 1);
		Ini_PutInt (ini, sectionName, INI_TAG_CCDSETTINGS_saveData, c->saveData);    
	}
	else {
		Ini_PutInt (ini, sectionName, INI_TAG_CCDSETTINGS_picturesPerRepetition, c->picturesPerRepetition);
	}
		

	
	Ini_PutInt (ini, sectionName, INI_TAG_CCDSETTINGS_exposureTime_ms, c->exposureTime_ms);
	
	Ini_PutInt0 (ini, sectionName, INI_TAG_CCDSETTINGS_enableEMCCD, c->enableEMCCD);
	Ini_PutInt (ini, sectionName, INI_TAG_CCDSETTINGS_EMCCDgain, c->EMCCDgain);
	Ini_PutInt (ini, sectionName, INI_TAG_CCDSETTINGS_exposureTime_ms, c->exposureTime_ms);
	
	Ini_PutInt (ini, sectionName, INI_TAG_CCDSETTINGS_takeSubimage, c->takeSubimage);
	Ini_PutInt0 (ini, sectionName, INI_TAG_CCDSETTINGS_subimageCenterX, c->subimageCenterX);
	Ini_PutInt0 (ini, sectionName, INI_TAG_CCDSETTINGS_subimageCenterY, c->subimageCenterY);
	Ini_PutInt0 (ini, sectionName, INI_TAG_CCDSETTINGS_subimageSizeX, c->subimageSizeX);
	Ini_PutInt0 (ini, sectionName, INI_TAG_CCDSETTINGS_subimageSizeY, c->subimageSizeY);
	Ini_PutInt (ini, sectionName, INI_TAG_CCDSETTINGS_binsize, c->binsize);
	Ini_PutInt (ini, sectionName, INI_TAG_CCDSETTINGS_evaluateSubimage, c->evaluateSubimage);
	Ini_PutInt0 (ini, sectionName, INI_TAG_CCDSETTINGS_evaluateCenterX, c->evaluateCenterX);      
	Ini_PutInt0 (ini, sectionName, INI_TAG_CCDSETTINGS_evaluateCenterY, c->evaluateCenterY);
	Ini_PutInt0 (ini, sectionName, INI_TAG_CCDSETTINGS_evaluateSizeX, c->evaluateSizeX);
	Ini_PutInt0 (ini, sectionName, INI_TAG_CCDSETTINGS_evaluateSizeY, c->evaluateSizeY);
	Ini_PutInt0 (ini, sectionName, INI_TAG_CCDSETTINGS_saveRawData, c->saveRawData);          
	Ini_PutInt (ini, sectionName, INI_TAG_CCDSETTINGS_imageProcessing, c->imageProcessing);
	Ini_PutInt0 (ini, sectionName, INI_TAG_CCDSETTINGS_pixelCount, c->pixelCount);
	Ini_PutInt0 (ini, sectionName, INI_TAG_CCDSETTINGS_sumPictures, c->sumPictures);
	Ini_PutInt0 (ini, sectionName, INI_TAG_CCDSETTINGS_separateFilePerRepetition, 
				 c->separateFilePerRepetition);

	
}




void Ini_GetCCDSettings (IniText ini, t_sequence *seq, int nr)
{
    char sectionName[30];
	t_ccdSettings *c;
	
	sprintf (sectionName, INI_SECTION_CCDSETTINGS, nr);
	c = CCDSETTINGS_new (seq);
	CCDSETTINGS_init (c);
	
	Ini_GetInt (ini, sectionName, INI_TAG_CCDSETTINGS_enableCCD, 
				&c->enableCCD);
	Ini_GetInt (ini, sectionName, INI_TAG_CCDSETTINGS_serialNo, 
				&c->serialNo);
	Ini_GetStringIntoBuffer (ini, sectionName,
							 INI_TAG_CCDSETTINGS_description,
							 c->description, MAX_CCDDESCRIPTION_LEN);
	Ini_GetInt (ini, sectionName, INI_TAG_CCDSETTINGS_cameraType,
				&c->cameraType);
	Ini_GetInt (ini, sectionName, INI_TAG_CCDSETTINGS_shutter, 
				&c->shutter);
	Ini_GetInt (ini, sectionName, INI_TAG_CCDSETTINGS_cooler, 
				&c->cooler);
	Ini_GetInt (ini, sectionName, INI_TAG_CCDSETTINGS_fanMode, 
				&c->fanMode);
	Ini_GetInt (ini, sectionName, INI_TAG_CCDSETTINGS_temperature, 
				&c->temperature);
	Ini_GetInt (ini, sectionName, INI_TAG_CCDSETTINGS_acquisition, 
				&c->acquisition);
	Ini_GetInt (ini, sectionName, INI_TAG_CCDSETTINGS_preamplifierGain, 
				&c->preamplifierGain);
	Ini_GetInt (ini, sectionName, INI_TAG_CCDSETTINGS_shiftSpeed, 
				&c->shiftSpeed);
	Ini_GetInt (ini, sectionName, INI_TAG_CCDSETTINGS_readoutRate, 
				&c->readoutRate);
	Ini_GetInt (ini, sectionName, INI_TAG_CCDSETTINGS_picturesPerRepetition, 
				&c->picturesPerRepetition);
	Ini_GetInt (ini, sectionName, INI_TAG_CCDSETTINGS_exposureTime_ms, 
				&c->exposureTime_ms);
	Ini_GetInt (ini, sectionName, INI_TAG_CCDSETTINGS_enableEMCCD, 
				&c->enableEMCCD);
	Ini_GetInt (ini, sectionName, INI_TAG_CCDSETTINGS_EMCCDgain, 
				&c->EMCCDgain);
	Ini_GetInt (ini, sectionName, INI_TAG_CCDSETTINGS_takeSubimage, 
				&c->takeSubimage);
	Ini_GetInt (ini, sectionName, INI_TAG_CCDSETTINGS_subimageCenterX, 
				&c->subimageCenterX);
	Ini_GetInt (ini, sectionName, INI_TAG_CCDSETTINGS_subimageCenterY, 
				&c->subimageCenterY);
	Ini_GetInt (ini, sectionName, INI_TAG_CCDSETTINGS_subimageSizeX, 
				&c->subimageSizeX);
	Ini_GetInt (ini, sectionName, INI_TAG_CCDSETTINGS_subimageSizeY, 
				&c->subimageSizeY);
	Ini_GetInt (ini, sectionName, INI_TAG_CCDSETTINGS_binsize, 
				&c->binsize);
	Ini_GetInt (ini, sectionName, INI_TAG_CCDSETTINGS_evaluateSubimage, 
				&c->evaluateSubimage);
	Ini_GetInt (ini, sectionName, INI_TAG_CCDSETTINGS_evaluateCenterX, 
				&c->evaluateCenterX);      
	Ini_GetInt (ini, sectionName, INI_TAG_CCDSETTINGS_evaluateCenterY, 
				&c->evaluateCenterY);
	Ini_GetInt (ini, sectionName, INI_TAG_CCDSETTINGS_evaluateSizeX, 
				&c->evaluateSizeX);
	Ini_GetInt (ini, sectionName, INI_TAG_CCDSETTINGS_evaluateSizeY, 
				&c->evaluateSizeY);
	Ini_GetInt (ini, sectionName, INI_TAG_CCDSETTINGS_saveRawData, 
				&c->saveRawData);          
	Ini_GetInt (ini, sectionName, INI_TAG_CCDSETTINGS_imageProcessing, 
				&c->imageProcessing);
	Ini_GetInt (ini, sectionName, INI_TAG_CCDSETTINGS_pixelCount,
				&c->pixelCount);
	Ini_GetInt (ini, sectionName, INI_TAG_CCDSETTINGS_sumPictures, 
				&c->sumPictures);
	Ini_GetInt (ini, sectionName, INI_TAG_CCDSETTINGS_separateFilePerRepetition, 
				&c->separateFilePerRepetition);
	
}







//=======================================================================
//
//    check if waveforms are equal
//
//=======================================================================
void eliminateIdenticalWaveforms (t_sequence *seq)
{
    int i, j, ch, newNr;
    
    t_waveform *w, *same;
    t_digitalBlock *d;

    for (i = ListNumItems (seq->lWaveforms); i > 1; i--) {
        w = WFM_ptr(seq, i);
		// -----------------------------------------
	    //    check if name begins with "_"
		// -----------------------------------------
        if (w->name[0] == '_') {
            newNr = WFM_nrFromName (seq, w->name+1, i-1);
            if (newNr > 0) {
                same =  WFM_ptr(seq, newNr); 
           		if (WFM_isEqual (w, same)) {
//        		    DebugPrintf ("WFM equal: (%d)%s =(%d)%s\n", i, w->name, newNr, same->name);
					// -----------------------------------------
				    //    change all links to this waveform
					// -----------------------------------------
					for (j = 1; j <= ListNumItems (seq->lDigitalBlocks); j++) {
						d = DIGITALBLOCK_ptr (seq, j);
				        for (ch = 0; ch < N_DAC_CHANNELS; ch++) { 
				            if (d->waveforms[ch] == i) {
				                d->waveforms[ch] = newNr;
                
				            }
				            if (d->waveforms[ch] > i)
				                d->waveforms[ch] --;
				        }
				    }
				    Breakpoint();
				    // change numbers also of added waveforms
				    
					// -----------------------------------------
				    //    delete duplicate waveform
					// -----------------------------------------
					WFM_delete (seq, i); 
//					setChanges (seq, 1);
				 }        		    
            }
        }
    }
}





int Ini_GetSequence (IniText ini, t_sequence *seq)
{
    int nDigitalBlocks = 0;
//    int nAnalogBlocks  = 0;
    int nWaveforms	   = 0;
	int nExternalDevices =0;
    int nGpibCommands  = 0;
    int nTransferFunctions = 0;
	int nCCDSettings = 0;

    //int nCounters = 0;
    int i;
    int progressDialog = 0;

 
	if (seq == NULL) return -1;

	Ini_GetDouble (ini, INI_SECTION_FILEINFO, INI_TAG_FILEINFO_Version, &seq->programVersionCreatedWith);


	// DAC Setup
	Ini_GetDACSetup (ini, seq);
	
	// DIO Setup
	Ini_GetDIOSetup (ini, seq);

	// user interface setup
	Ini_GetTableConfig (ini, &seq->tableConfig, 0);


	if (strlen (seq->filename) == 0) {
		Ini_GetStringIntoBuffer (ini, INI_SECTION_FILEINFO,
								INI_TAG_FILEINFO_filename,
								seq->filename, MAX_PATHNAME_LEN);
	}

	
	//Ini_GetInt (ini, INI_SECTION_SEQUENCEINFO, INI_TAG_FILEINFO_startNr, &seq->startNr);
	
	Ini_GetStringCopy (ini, INI_SECTION_SEQUENCEINFO, INI_TAG_SEQUENCEINFO_description,
				  &seq->description);
	Ini_GetInt (ini, INI_SECTION_SEQUENCEINFO, INI_TAG_SEQUENCEINFO_nRuns,
				&seq->nRuns);
	Ini_GetInt (ini, INI_SECTION_SEQUENCEINFO, INI_TAG_SEQUENCEINFO_nRepetitions,
				&seq->nRepetitions);
	Ini_GetInt (ini, INI_SECTION_SEQUENCEINFO, INI_TAG_SEQUENCEINFO_nCopies,
				&seq->nCopies);
	if (seq->nCopies < 1) seq->nCopies = 1;
	seq->redoRepetitions = Ini_GetDataArray (ini, INI_SECTION_SEQUENCEINFO, INI_TAG_SEQUENCEINFO_redoRepetitions,
					  						 &seq->nRedo, VAL_INTEGER);
	seq->redoRuns = Ini_GetDataArray (ini, INI_SECTION_SEQUENCEINFO, INI_TAG_SEQUENCEINFO_redoRuns,
					  						 &seq->nRedo, VAL_INTEGER);
	Ini_GetInt (ini, INI_SECTION_SEQUENCEINFO, INI_TAG_SEQUENCEINFO_considerWatchdog,
				&seq->considerWatchdog);
	Ini_GetInt (ini, INI_SECTION_SEQUENCEINFO, INI_TAG_SEQUENCEINFO_generateRandomCounterData,
				&seq->generateRandomCounterData);
	Ini_GetStringIntoBuffer (ini, INI_SECTION_SEQUENCEINFO, INI_TAG_SEQUENCEINFO_seqShortName,
							 seq->seqShortName, MAX_PATHNAME_LEN);
	if (seq->seqShortName[0] == 0) {
		strncpy (seq->seqShortName, extractFilename (seq->filename), 30);		
	}
	changeSuffix(seq->seqShortName, seq->seqShortName, "");
	
	// Waveforms
	Ini_GetInt (ini, INI_SECTION_SEQUENCEINFO, INI_TAG_SEQUENCEINFO_nWaveforms,
				&nWaveforms);
	for (i = 0; i < nWaveforms; i++) Ini_GetWaveform (ini, seq, i+1);
	WFM_checkForDuplicateNames (seq);
	
	Ini_GetInt (ini, INI_SECTION_SEQUENCEINFO, INI_TAG_SEQUENCEINFO_wfmTableSortMode,
				&seq->wfmTableSortMode);
	
	// External Devices
	Ini_GetInt (ini, INI_SECTION_SEQUENCEINFO, INI_TAG_SEQUENCEINFO_nExternalDevices,
				&nExternalDevices);
	for (i = 0; i < nExternalDevices; i++) Ini_GetExternalDevice (ini, seq, i+1);
	//EXTDEV_checkForDuplicateNames (seq);	   Do I need this?
	Ini_GetInt (ini, INI_SECTION_SEQUENCEINFO, INI_TAG_SEQUENCEINFO_extdevTableSortMode,
				&seq->extdevTableSortMode);


	
	// DigitalBlocks
	
	Ini_GetInt (ini, INI_SECTION_SEQUENCEINFO, INI_TAG_SEQUENCEINFO_nDigitalBlocks, 
				&nDigitalBlocks);
	if (nDigitalBlocks > 500) 
		progressDialog = CreateProgressDialog ("Retrieving sequence information", "", 1, VAL_NO_MARKERS, "");
	for (i = 0; i < nDigitalBlocks; i++) {
		Ini_GetDigitalBlock (ini, seq, i+1);
		if ((i % 20 == 0) && (progressDialog > 0)) 
			UpdateProgressDialog (progressDialog, 100 * i / nDigitalBlocks, 0);
	}
	if (progressDialog > 0) UpdateProgressDialog (progressDialog, 100, 0);

	// transfer functions
	Ini_GetInt (ini, INI_SECTION_SEQUENCEINFO, INI_TAG_SEQUENCEINFO_nTransferFunctions,
				&nTransferFunctions);
	for (i = 0; i < nTransferFunctions; i++) Ini_GetTransferFunction (ini, seq, i+1);

	// Gpib Commands
	Ini_GetInt (ini, INI_SECTION_SEQUENCEINFO, INI_TAG_SEQUENCEINFO_nGpibCommands,
				&nGpibCommands);
	for (i = 0; i < nGpibCommands; i++) Ini_GetGpibCommand (ini, seq, i+1);
	// CCD Settings
	Ini_GetInt (ini, INI_SECTION_SEQUENCEINFO, INI_TAG_SEQUENCEINFO_nCCDSettings,
				&nCCDSettings);
	for (i = 0; i < nCCDSettings; i++) Ini_GetCCDSettings (ini, seq, i+1);

	// update from V4.37 to V5.00 --> waveforms must be associated to DAC channels
	WFM_addChannels (seq);
	
	// Focus feedback
	Ini_GetInt (ini, INI_SECTION_SEQUENCEINFO, INI_TAG_SEQUENCEINFO_enableFocusFeedback,
				&seq->enableFocusFeedback);
	Ini_GetDouble (ini, INI_SECTION_SEQUENCEINFO, INI_TAG_SEQUENCEINFO_manualPifocFocusVoltage,
				&seq->manualPifocFocusVoltage);
	Ini_GetInt (ini, INI_SECTION_SEQUENCEINFO, INI_TAG_SEQUENCEINFO_autoFocusNumImages,
				&seq->autoFocusNumImages);
	Ini_GetDouble (ini, INI_SECTION_SEQUENCEINFO, INI_TAG_SEQUENCEINFO_autoFocusCenterVoltage,
				&seq->autoFocusCenterVoltage);
	Ini_GetDouble (ini, INI_SECTION_SEQUENCEINFO, INI_TAG_SEQUENCEINFO_autoFocusStepVoltage,
				&seq->autoFocusStepVoltage);
	Ini_GetInt (ini, INI_SECTION_SEQUENCEINFO, INI_TAG_SEQUENCEINFO_autoFocusFirstBlock,
				&seq->autoFocusFirstBlock);
	Ini_GetInt (ini, INI_SECTION_SEQUENCEINFO, INI_TAG_SEQUENCEINFO_isOfTypeIdleSeq,
				&seq->isOfTypeIdleSeq);
	
	
	
	if (seq->manualPifocFocusVoltage < 0 || seq->manualPifocFocusVoltage > 10) {
		seq->manualPifocFocusVoltage = 0;
	}

	if (progressDialog) DiscardProgressDialog(progressDialog);

	return 0;
}




/******************************************************************************/
/*
/*  load "*.seq"-file into memory
/*
/******************************************************************************/


// set digital channel voltage to zero  if first block is "unchanged"
void SEQUENCE_addVoltageZeroToFirstBlock (t_sequence *seq)
{
	int ch;
	t_digitalBlock *b;
	
	if (seq == NULL) return;
	
	if (ListNumItems (seq->lDigitalBlocks) == 0) return;
	ListGetItem (seq->lDigitalBlocks, &b, 1);
	for (ch = 0; ch < N_DAC_CHANNELS; ch++) {
		if (b->waveforms[ch] == WFM_ID_UNCHANGED) {
			b->waveforms[ch] = WFM_ID_CONSTVOLTAGE;
			b->constVoltages[ch] = 0.0;
		}
	}
	
}



int SEQUENCE_load (const char *filename, t_sequence *seq)
{
    int err = 0;
    char tmpFilename[MAX_PATHNAME_LEN];
    IniText ini = NULL;
    double version;
	int month, day, year;
   
    SEQUENCE_init (seq);

	if (strlen (filename) == 0) return -1;
	if ((ini = Ini_New (1)) == 0) return -1;
//	Ini_SetDuplicateChecking (ini, 0);
	
	SetWaitCursor (1);
	
	if ((err = Ini_ReadFromFile (ini, filename)) != 0) {
		displayIniFileError (filename, err);
		goto ERRORID;
	}
	
	if (Ini_GetDouble (ini, INI_SECTION_FILEINFO, INI_TAG_FILEINFO_Version, &version) == 0) {
		// sequence saved by old program version
		// ==> convert it into the new type
	   	Ini_Dispose (ini); 
	   	ini = NULL;
		if ((ini = Ini_New (1)) == 0) goto ERRORID;
//		Ini_SetDuplicateChecking (ini, 0);
	   	changeSuffix (tmpFilename, filename, "_convert.tmp");
	   	convertSequenceFile_OldPrgVersion (filename, tmpFilename);
		if ((err = Ini_ReadFromFile (ini, tmpFilename)) != 0) {
			displayIniFileError (tmpFilename, err);
			goto ERRORID; 
		}
		DeleteFile (tmpFilename);
	}

	
    strcpy (seq->filename,filename);
	Ini_GetSequence (ini, seq);	
	
	// -----------------------------------------
    //    eliminates indentical waveforms 
    //    Bugfix for wrong "import"										
	// -----------------------------------------
	eliminateIdenticalWaveforms (seq);
	
	
	if (ListNumItems(seq->lDigitalBlocks) == 0) {
    	DIGITALBLOCK_init (DIGITALBLOCK_new (seq));
	}
	
	GetFileDate (filename, &month, &day, &year);
	SEQUENCE_addDateLastModified (seq, month, day, year);
	isFileWriteProtected (filename, &seq->readOnly);
	SEQUENCE_addVoltageZeroToFirstBlock (seq);

ERRORID:
	SetWaitCursor (0);
	Ini_Dispose (ini);
	
	
	return err;
}



int Ini_PutSequence (IniText ini, t_sequence *seq)
{
	int i, n;
	int progressDialog = 0;
	
	if (seq == NULL) return -1;

	Ini_SetDuplicateChecking (ini, 0);
	Ini_PutString (ini, INI_SECTION_FILEINFO, INI_TAG_FILEINFO_filename,
			extractFilename (seq->filename));

	seq->programVersionCreatedWith = ProgramVersion;
	Ini_PutDouble (ini, INI_SECTION_FILEINFO, INI_TAG_FILEINFO_Version, seq->programVersionCreatedWith);

	if (seq->startNr > 0) 
	    Ini_PutInt (ini, INI_SECTION_FILEINFO, INI_TAG_FILEINFO_startNr, seq->startNr);

	
	
	Ini_PutDACSetup (ini, seq);
	Ini_PutDIOSetup (ini, seq);
	Ini_PutTableConfig (ini, &seq->tableConfig, 0);

	if (seq->description != NULL) Ini_PutString (ini, INI_SECTION_SEQUENCEINFO, INI_TAG_SEQUENCEINFO_description,
				seq->description);
	Ini_PutInt (ini, INI_SECTION_SEQUENCEINFO, INI_TAG_SEQUENCEINFO_nRuns,
				seq->nRuns);
	Ini_PutInt (ini, INI_SECTION_SEQUENCEINFO, INI_TAG_SEQUENCEINFO_nRepetitions,
				seq->nRepetitions);
	Ini_PutInt (ini, INI_SECTION_SEQUENCEINFO, INI_TAG_SEQUENCEINFO_nCopies,
				seq->nCopies);
	Ini_PutDataArray (ini, INI_SECTION_SEQUENCEINFO, INI_TAG_SEQUENCEINFO_redoRepetitions,
					  seq->redoRepetitions, seq->nRedo, VAL_INTEGER);
	Ini_PutDataArray (ini, INI_SECTION_SEQUENCEINFO, INI_TAG_SEQUENCEINFO_redoRuns,
					  seq->redoRuns, seq->nRedo, VAL_INTEGER);
	Ini_PutInt (ini, INI_SECTION_SEQUENCEINFO, INI_TAG_SEQUENCEINFO_considerWatchdog,
				seq->considerWatchdog);
				
	Ini_PutInt0 (ini, INI_SECTION_SEQUENCEINFO, INI_TAG_SEQUENCEINFO_generateRandomCounterData,
				seq->generateRandomCounterData);
//				generateRandomCounterData

	Ini_PutString (ini, INI_SECTION_SEQUENCEINFO, INI_TAG_SEQUENCEINFO_seqShortName,
				seq->seqShortName);
	
	Ini_PutInt (ini, INI_SECTION_SEQUENCEINFO, INI_TAG_SEQUENCEINFO_nDigitalBlocks,
				ListNumItems (seq->lDigitalBlocks));
	n = ListNumItems (seq->lDigitalBlocks);
	if (n > 500) progressDialog = CreateProgressDialog ("Storing sequence information",
																   "", 1, VAL_NO_MARKERS, "");
	for (i = 1; i <= n; i++) {
		Ini_PutDigitalBlock (ini, seq, i);
		if ((i % 20 == 0) && (progressDialog > 0)) 
			UpdateProgressDialog (progressDialog, 100 * i / n, 0);
	}
	UpdateProgressDialog (progressDialog, 100, 0);
	
//Waveforms
	Ini_PutInt (ini, INI_SECTION_SEQUENCEINFO, INI_TAG_SEQUENCEINFO_nWaveforms,
				ListNumItems (seq->lWaveforms));
	for (i = 1; i <= ListNumItems (seq->lWaveforms); i++) 
		Ini_PutWaveform (ini, seq, i);
	
	Ini_PutInt (ini, INI_SECTION_SEQUENCEINFO, INI_TAG_SEQUENCEINFO_wfmTableSortMode,
				seq->wfmTableSortMode);
//External Devices
	Ini_PutInt (ini, INI_SECTION_SEQUENCEINFO, INI_TAG_SEQUENCEINFO_nExternalDevices,
				ListNumItems (seq->lExtDevices));
	for (i = 1; i <= ListNumItems (seq->lExtDevices); i++) 
		Ini_PutExternalDevice (ini, seq, i);
	
	Ini_PutInt (ini, INI_SECTION_SEQUENCEINFO, INI_TAG_SEQUENCEINFO_extdevTableSortMode,
				seq->extdevTableSortMode);
	
	
	Ini_PutInt (ini, INI_SECTION_SEQUENCEINFO, INI_TAG_SEQUENCEINFO_nGpibCommands,
				ListNumItems (seq->lGpibCommands));
	for (i = 1; i <= ListNumItems (seq->lGpibCommands); i++) 
		Ini_PutGpibCommand (ini, seq, i);

	if (ListNumItems (seq->lTransferFunctions) > 0) {
		Ini_PutInt (ini, INI_SECTION_SEQUENCEINFO, INI_TAG_SEQUENCEINFO_nTransferFunctions,
					ListNumItems (seq->lTransferFunctions));
		for (i = 1; i <= ListNumItems (seq->lTransferFunctions); i++) 
			Ini_PutTransferFunction (ini, seq, i);
	}

	if (ListNumItems (seq->lCCDSettings) > 0) {
		Ini_PutInt (ini, INI_SECTION_SEQUENCEINFO, INI_TAG_SEQUENCEINFO_nCCDSettings,
					ListNumItems (seq->lCCDSettings));
		for (i = 1; i <= ListNumItems (seq->lCCDSettings); i++) 
			Ini_PutCCDSettings (ini, seq, i, 0);
	}
	
	// focus feedback
	Ini_PutInt (ini, INI_SECTION_SEQUENCEINFO, INI_TAG_SEQUENCEINFO_enableFocusFeedback,
			seq->enableFocusFeedback);
	Ini_PutDouble (ini, INI_SECTION_SEQUENCEINFO, INI_TAG_SEQUENCEINFO_manualPifocFocusVoltage,
			seq->manualPifocFocusVoltage);
	Ini_PutInt (ini, INI_SECTION_SEQUENCEINFO, INI_TAG_SEQUENCEINFO_autoFocusNumImages,
				seq->autoFocusNumImages);
	Ini_PutDouble (ini, INI_SECTION_SEQUENCEINFO, INI_TAG_SEQUENCEINFO_autoFocusCenterVoltage,
				seq->autoFocusCenterVoltage);
	Ini_PutDouble (ini, INI_SECTION_SEQUENCEINFO, INI_TAG_SEQUENCEINFO_autoFocusStepVoltage,
				seq->autoFocusStepVoltage);
	Ini_PutInt (ini, INI_SECTION_SEQUENCEINFO, INI_TAG_SEQUENCEINFO_autoFocusFirstBlock,
				seq->autoFocusFirstBlock);
	Ini_PutInt (ini, INI_SECTION_SEQUENCEINFO, INI_TAG_SEQUENCEINFO_isOfTypeIdleSeq,
				seq->isOfTypeIdleSeq);
	
	if (progressDialog) DiscardProgressDialog (progressDialog);
	return 0;
}



/*******************************************************************************/
/*
/*  save "*.seq"-file 
/*
/******************************************************************************/


int Ini_writeFile (IniText ini, const char *filename)
{
	int err;
	

	if ((err = Ini_WriteToFile(ini, filename)) != 0) {
		return displayIniFileError (filename, err);
	}
	return 0;
}


// clone sequence num times
t_sequence** SEQUENCE_clone_multi(t_sequence *seq, int num) {
	/*t_sequence *clonedSeq;
	IniText ini;
	int i;
	
	if (num < 1) {
		return NULL;	
	}
	
	if ((ini = Ini_New (0)) == 0) return NULL;      
	
	clonedSeq = (t_sequence *) malloc (num*sizeof(t_sequence));
	if (clonedSeq == NULL) {
		return NULL;
	}
		
	Ini_PutSequence (ini, seq);
	
	for (i = 0; i < num; i++) {
		SEQUENCE_init (&clonedSeq[i]);
		Ini_GetSequence(ini, &clonedSeq[i]);
	}
	Ini_Dispose (ini);
	return clonedSeq;  */
	
	t_sequence **clonedSeq;
	IniText ini;
	int i;
	
	if (num < 1) {
		return NULL;	
	}
	
	if ((ini = Ini_New (0)) == 0) return NULL;      
	
	clonedSeq = (t_sequence **) malloc (num*sizeof(t_sequence*));
	if (clonedSeq == NULL) {
		return NULL;
	}
		
	Ini_PutSequence (ini, seq);
	
	for (i = 0; i < num; i++) {
		clonedSeq[i] = SEQUENCE_new();
		SEQUENCE_init (clonedSeq[i]);
		Ini_GetSequence(ini, clonedSeq[i]);
	}
	Ini_Dispose (ini);
	return clonedSeq;
}

// clones the sequence via exporting to ini and reading ini object again
t_sequence* SEQUENCE_clone(t_sequence *seq)
{
	t_sequence **tmp = SEQUENCE_clone_multi(seq, 1);
	t_sequence *ret = *tmp;
	free(tmp);
	return ret; 
	/*
	t_sequence *clonedSeq;
	IniText ini;  
	
	if ((ini = Ini_New (0)) == 0) return NULL;      
	
	clonedSeq = SEQUENCE_new ();
	if (clonedSeq == NULL) {
		return NULL;
	}
	SEQUENCE_init (clonedSeq);
	
	Ini_PutSequence (ini, seq);
	
	Ini_GetSequence(ini, clonedSeq);
	
		// -----------------------------------------
	    //    eliminates indentical waveforms 
	    //    Bugfix for wrong "import"										
		// -----------------------------------------
		eliminateIdenticalWaveforms (seq);
	
	
		if (ListNumItems(seq->lDigitalBlocks) == 0) {
	    	DIGITALBLOCK_init (DIGITALBLOCK_new (seq));
		}
		SEQUENCE_addVoltageZeroToFirstBlock (seq);
		// not clear if these lines are needed ...
	
	
	Ini_Dispose (ini); 
	return clonedSeq;*/
	
}

int SEQUENCE_save (const char *filename, t_sequence *seq)
{
    int err;
    IniText ini;
	char msg[500];
	char newFilename[MAX_PATHNAME_LEN] = "";
	int overwrite = 0;

	if ((ini = Ini_New (0)) == 0) return -1;
	
	if (isFileWriteProtected (filename, 0)) {
		sprintf (msg, "Attribute of file '%s'\nis set to 'read only'!", seq->filename);
		switch (PostGenericMessagePopup ("Warning", msg, "Overwrite",
									   "Save sequence as", "Abort", 0,
									   0, 0, VAL_GENERIC_POPUP_BTN3,
									   VAL_GENERIC_POPUP_BTN3,
									   VAL_GENERIC_POPUP_BTN3)) {
			case VAL_GENERIC_POPUP_BTN1: // Overwrite
				err = SetFileAttrs (filename, 0, -1, -1, -1);
				if (err != 0) {
					displayIniFileError (filename, err);
					return err;
				}
				overwrite = 1;
				break;

			case VAL_GENERIC_POPUP_BTN2: // save as
			    if (FileSelectPopup (config->defaultPath, filename, SEQUENCE_fileSuffix,
                        "Save sequence as", VAL_SAVE_BUTTON,
                         0, 0, 1, 1, newFilename) <= 0) return -1;
				filename = newFilename;
				break;
			case VAL_GENERIC_POPUP_BTN3: // abort
				return -1;
		}
	}
	
	Ini_PutSequence (ini, seq);
	
	err = Ini_writeFile (ini, filename);
	if (overwrite) SetFileAttrs (filename, 1, -1, -1, -1);
	if (err == 0) seq->changes = 0;
	
	Ini_Dispose (ini);
    return err;
}



// format of arrivaltimes buffer
// 4 bytes: nCurves
// nnCurves * 4 bytes = (data->nArrivalTimes)
// then arrivaltimes







void setLaunchState (int state)
{
	RegWriteULong (REGKEY_HKLM, CONF_REG_SUBKEY_EXE, CONF_TAG_LAUNCHED,
				   state, 0);
}


int getLaunchState (void)
{
	unsigned state = 0;
	
	if (RegReadULong (REGKEY_HKLM, CONF_REG_SUBKEY_EXE, CONF_TAG_LAUNCHED,
				      &state, 0) == 0) return state;
	return state;
}








