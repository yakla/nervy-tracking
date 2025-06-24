#include "devices.h"
#include "touchpad.h"
#include "config.h"
#include <string>

bool checkInput(UINT rawInputSize, PRAWINPUT rawInputData, hidDeviceInfo& deviceInfo) {
	if (rawInputData->header.dwType != RIM_TYPEHID)
		return false;

	DWORD count = rawInputData->data.hid.dwCount;
	BYTE* rawData = rawInputData->data.hid.bRawData;

	if (count == 0)
		return false;

	std::wstring deviceName;
	getRawInputDeviceName(rawInputData->header.hDevice, deviceName);

	unsigned int foundHidIdx = (unsigned int)-1;

	for (unsigned int touchpadIndex = 0; touchpadIndex < deviceInfoList.size(); touchpadIndex++)
	{
		int compareNameResult = deviceName.compare(deviceInfoList[touchpadIndex].name);
		if (compareNameResult == 0)
		{
			//wprintf(deviceInfoList[touchpadIndex].name.c_str());
			foundHidIdx = touchpadIndex;
			deviceInfo = deviceInfoList[foundHidIdx];

			if (deviceInfo.linkCollectionInfoList.empty())
			{
				printf(FG_RED);
				printf("Cannot find any LinkCollection(s). Try parse the PREPARED_DATA may help. TODO\n");
				printf(RESET_COLOR);
				return false;
			}
			else if (deviceInfo.preparsedData == nullptr)
			{
				printf(FG_RED);
				printf("Cannot find PreparsedData\n");
				printf(RESET_COLOR);
				return false;
			}
			return true;
		}
	}

	return false;
}

bool readInput(UINT rawInputSize, PRAWINPUT rawInputData, hidDeviceInfo& deviceInfo, std::vector<TouchData>& touchPoints) {
	NTSTATUS hidpReturnCode;
	ULONG usageValue;
	PHIDP_PREPARSED_DATA preparsedHIDData = deviceInfo.preparsedData;

	if (deviceInfo.contactCountLinkCollection == (USHORT)-1)
	{
		printf(FG_RED);
		printf("Cannot find contact count Link Collection!\n");
		printf(RESET_COLOR);
		return false;
	}

	hidpReturnCode = HidP_GetUsageValue(HidP_Input, HID_USAGE_PAGE_DIGITIZER, deviceInfo.contactCountLinkCollection, HID_USAGE_DIGITIZER_CONTACT_COUNT, &usageValue, preparsedHIDData, (PCHAR)rawInputData->data.hid.bRawData, rawInputData->data.hid.dwSizeHid);

	if (hidpReturnCode != HIDP_STATUS_SUCCESS)
	{
		printf(FG_RED);
		printf("Failed to read number of contacts!\n");
		printf(RESET_COLOR);
		printHidPErrors(hidpReturnCode);
		return false;
	}

	hidTouchLinkCollectionInfo collectionInfo = deviceInfo.linkCollectionInfoList[0];

	BYTE *data = rawInputData->data.hid.bRawData;
	/*for (size_t i = 0; i < 42; i++)
	{
		printf("%02x ", data[i]);
	}
	printf("\n");*/
	USHORT fingerAmount = data[1]/16;  //finger amount
	USHORT OPCODE = data[0]; // opcode
	USHORT Timestamp = data[3]; //just a recording of the time stamp
	USHORT touchFlag = data[4]; // the way it works 3 if in center 2 if in edges and 1 at center top/bottom 2 at edges of x . and the first value is the touchID
	for (size_t i = 0; i < fingerAmount; i++){
		auto currentTouchID = (data[4 + i * 5] >> 4);
			touchPoints[currentTouchID].x = data[5 + i * 5] | (data[6 + i * 5] << 8);
			touchPoints[currentTouchID].y = data[7 + i * 5] | (data[8 + i * 5] << 8);
			touchPoints[currentTouchID].onSurface = ((data[4 + i * 5] & 3) == 3 || (data[4 + i * 5] & 2) == 2);
			touchPoints[currentTouchID].touchID = currentTouchID;
			touchPoints[currentTouchID].wasUpdatedThisFrame = true;
	}
	for (size_t i = 0; i < maxTouchPoints; i++) {
		if (!touchPoints[i].wasUpdatedThisFrame) {
			touchPoints[i].x = 0;
			touchPoints[i].y = 0;
			touchPoints[i].onSurface = false;
			touchPoints[i].touchID = -1;
		}
	}

	touchPoints[0].numberOfFingersOnTrackPad = data[1] / 16;
	touchPoints[0].timestamp = data[3];
		return true;
}