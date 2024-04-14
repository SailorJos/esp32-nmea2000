/*
  GWbattvoltTask.cpp  by JS
*/

#ifdef ttgo-t7-v14-mini32

#include "GwApi.h"
#include "GwbattvoltTask.h"
#include "N2kMessages.h"
#include "GwXdrTypeMappings.h"
#include "GWConfig.h"
#include <vector>
#include "GwTimer.h"

#define inputPin 35

void battvoltTask(GwApi *param);

//DECLARE_USERTASK(battvoltTask);
DECLARE_USERTASK_PARAM(battvoltTask, 3000);


//---------------------------------------------------------------------------------------------
//// an init function that ist being called before other initializations from the core
void battvoltInit(GwApi *api) {
  api->getLogger()->logDebug(GwLog::LOG, "battvolt init run");
  // make the task known to the core
  // the task function should not return (unless you delete the task - see example code)
  const String taskName("battvoltTask");
  api->addUserTask(battvoltTask, taskName, 3000);
  // this would create our task with a stack size of 4000 bytes

  // we declare some capabilities that we can
  // use in config.json to only show some
  // elements when this capability is set correctly
  //api->addCapability("ttgo-t7-v14-mini32", "true");
  //api->addCapability("ttgo-t7-v14-mini32_2", "true");
  // hide some config value
  // and force it's default value
  auto current = api->getConfig()->getConfigItem(GwConfigDefinitions::minXdrInterval, false);
  String defaultXdrInt = "50";
  if (current) {
    defaultXdrInt = current->getDefault();
  }
  //with the true parameter this config value will be hidden
  //if you would like the user to be able to see this item, omit the "true", the config value will be read only
  api->getConfig()->setValue(GwConfigDefinitions::minXdrInterval, defaultXdrInt, true);
  //we would like to store data with the interfaces that we defined
  //the name must match the one we used for addUserTask
  api->taskInterfaces()->claim<battvoltTaskIf>(taskName);
  //check if we should simulate some voltage measurements
  //add an XDR mapping in this case
  String voltTransducer = api->getConfig()->getString(GwConfigDefinitions::ADconvertor);
  if (!voltTransducer.isEmpty()) {
    int instance = api->getConfig()->
                   getInt(GwConfigDefinitions::bvInstanceId);
    GwXDRMappingDef xdr;
    //we send a battery message - so it is category battery
    xdr.category = GwXDRCategory::XDRBAT;
    //we only need a conversion from N2K to NMEA0183
    xdr.direction = GwXDRMappingDef::Direction::M_FROM2K;
    //you can find the XDR field and selector definitions
    //in the generated GwXdrTypeMappings.h
    xdr.field = GWXDRFIELD_BATTERY_BATTERYVOLTAGE;
    //xdr.selector=-1; //refer to xdrconfig.json - there is no selector under Battery, so we can leave it empty
    //we just map exactly our instance
    xdr.instanceMode = GwXDRMappingDef::IS_SINGLE;

    //those are the user configured values
    //this instance is the one we use for the battery instance when we set up
    //the N2K message
    xdr.instanceId = instance;
    xdr.xdrName = voltTransducer;
    if (!api->addXdrMapping(xdr)) {
      api->getLogger()->logDebug(GwLog::ERROR, "unable to set our xdr mapping %s", xdr.toString().c_str());
    }
    else {
      api->getLogger()->logDebug(GwLog::LOG, "xdr mapping set %s", xdr.toString().c_str());
    };
  }
}


//---------------------------------------------------------------------------------------------
void battvoltTask(GwApi *api) {
  GwLog *logger = api->getLogger();
  LOG_DEBUG(GwLog::LOG, "battvoltTask started");
  //get some configuration data
  String boatItemName = api->getConfig()->getString(api->getConfig()->battvoltBDSel);
  //------
  //initialization goes here
  //------
  //LOG_DEBUG(GwLog::LOG, "battvolt switch is %s", battvoltSwitch ? "true" : "false");
  LOG_DEBUG(GwLog::LOG, "minXdrInterval=%d", api->getConfig()->getInt(api->getConfig()->minXdrInterval));
  GwApi::BoatValue *testValue = new GwApi::BoatValue(boatItemName);
  GwApi::BoatValue *valueList[] = {testValue};
  GwApi::Status status;
  int counter = api->addCounter("battvoltcount");
  int apiResult = 0;
  battvoltTaskIf e1 = api->taskInterfaces()->get<battvoltTaskIf>(apiResult);
  LOG_DEBUG(GwLog::LOG, "battvoltIf before rs=%d,v=%d,s=%s", apiResult, e1.count, e1.someValue.c_str());
  String voltTransducer = api->getConfig()->getString(GwConfigDefinitions::ADconvertor);
  int voltageInstance = api->getConfig()->getInt(GwConfigDefinitions::bvInstanceId);
  api->getBoatDataValues(1, valueList);

  int runningNumReadings = 0;       // the actual number of readings
  float total = 0;                  // the running total
  float average = 0;                // the average
  float factor = 123.4; 
  float bias   = -252; 

  float actualVoltMeasurement;
  unsigned long previousMillis = 0;

  int battvoltintv = api->getConfig()->getInt(GwConfigDefinitions::battvoltintv);
  //api->getLogger()->logDebug(GwLog::LOG, "battvoltintv from settings in BoatData in sec %d", battvoltintv);

  int numReadings = api->getConfig()->getInt(GwConfigDefinitions::numReadings);
  //api->getLogger()->logDebug(GwLog::LOG, "number ADreadings to be averaged %d", numReadings);

  int voltCorr = api->getConfig()->getInt(GwConfigDefinitions::voltCorr);
  //api->getLogger()->logDebug(GwLog::LOG, "Correction for n2k power line resistance in mV %d", voltCorr);

  while (1)
  {
    delay(1); //js necessary in order to change over to other tasks !!!!!!!!!!!!!!!
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= (1000 * battvoltintv / numReadings) )
    {
      // save the last time
      previousMillis = currentMillis;
      total = total + analogRead(inputPin);
      runningNumReadings = runningNumReadings + 1;
      //LOG_DEBUG(GwLog::LOG, "runningNumReadings %d", runningNumReadings);
      if (runningNumReadings >= numReadings)
      {
        average = total / runningNumReadings;
        actualVoltMeasurement = (((average - bias) / factor) + float(voltCorr)/1000);
        LOG_DEBUG(GwLog::LOG, "actualVoltMeasurement %f", actualVoltMeasurement);
        LOG_DEBUG(GwLog::LOG, "average   %f", average);
        //LOG_DEBUG(GwLog::LOG, "runningNumReadings %d", runningNumReadings);
        runningNumReadings = 0;
        total = 0;
        tN2kMsg msg;
        SetN2kDCBatStatus(msg, voltageInstance, actualVoltMeasurement);

        //we send out an N2K message
        //and as we added an XDR mapping, we will see this in the data dashboard
        //and on the NMEA0183 stream
        api->sendN2kMessage(msg);
        ///        LOG_DEBUG(GwLog::LOG, "N2kMessage sent %f");

        //increment counter
        api->increment(counter, "battvoltcount");
      }
    }
  } //while(1)
  vTaskDelete(NULL);
}
#endif
