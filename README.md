# Azure Spatial Anchors Sample - Unreal
This project is an adaptation of the Unreal Mobile AR sample project to support Azure Spatial Anchors. It works with HoloLens 2, iOS (ARKit) and Android (ARCore) devices and supports multiplayer sessions.

> Note: Unreal Engine 4.25 contains the HoloLens 2 Azure Spatial Anchors plugin required for this sample. Subsequent releases of Unreal Engine will contain the needed iOS and Android Azure Spatial Anchors plugins. If you don't have access to Epic Game's Unreal Engine github repository, you will need to wait for releases after 4.25 for iOS and Android Azure Spatial Anchors support.

## Setup
1. Obtain a version of Unreal that supports Azure Spatial Anchors (Unreal 4.25+)
1. Clone this repo and retarget the AzureSpatialAnchorsSample.uproject to use your desired version of Unreal
1. Open this project with the Unreal editor and open `Content\HandheldARBP\Blueprints\GameFramework\BP_ARPawn.uasset`.
1. Set the `Azure Spatial Anchors Account Id` and `Azure Spatial Anchors Account Key` variables for the `BP_ARPawn` to match that of your Azure Spatial Anchors account. For more information on Azure Spatial Anchor account ids and keys, see [here](https://docs.microsoft.com/en-us/azure/spatial-anchors/concepts/authentication?tabs=csharp). Your project then should be ready for building and deploying to your desired device.

### HoloLens 2
1. Install the [Holographic Remoting application](https://www.microsoft.com/en-us/p/holographic-remoting-player/9nblggh4sv40?activetab=pivot:overviewtab) on your HoloLens 2 device.
1. Connect the Unreal Editor to your HoloLens 2 device running the Holographic Remoting app (Edit -> Project Settings -> Platforms -> Windows Mixed Reality).
1. Under play options in the Unreal editor select VR Preview.
1. Press play to run the experience on your HoloLens 2 device.

### Android
1. Build the Android package by running File -> Package Project -> Android -> (Multi:...).
1. Install the Android apk on your desired android device with the Unreal generated helper script.

### iOS
1. Cleanup any blueprint content related to windows mixed reality plugins that aren't available on Mac. This will require removing some blueprint nodes in the BP_ARPawn and the BP_MotionController.
1. Edit the iOS platform properties through the Unreal editor and setup your apple developer certificate and provisioning profile.
1. Package the application for iOS through Unreal.
1. Deploy the compiled *.ipa file to your desired iOS device through finder.

## Experience Overview
1. On application launch, devices will be placed in the SetupMap. A HoloLens 2 device will automatically load the HandheldARBlankMap as a listen server. For Android and iOS there will be two options. Mobile devices can select to run as server, which may not correctly accept clients, but will allow the device to run a single user Azure Spatial Anchors experience. If the mobile device instead selects to run as a client, the user can specify the ip address of the server (With holographic remoting, it is the ip address of the Editor PC) to connect to the shared experience.
1. Once devices are in the shared experience, they will be running the HandheldARBlankMap. In this map, an Azure Spatial Anchors session will be started once an AR Session is enabled. On HoloLens 2 this will be immediately. On Android and iOS, this will happen after the user chooses to start an AR Session through UI.
1. After starting an Azure Spatial Anchors session, users will be able to generate objects through gestures. A tap gesture on HoloLens will create an object. On mobile, touching a tracked plane geometry will generate an object.
1. Once an object is generated it will be white. After the Azure Spatial Anchor associated with said object is saved, the object will transition to green.
1. Tapping or touching a green object will cause it to turn blue and be deleted.

## TODOs
1. HoloLens 2 compilation to an appx has not been validated. The HoloLens 2 experience has only been tested in the editor with Holographic Remoting. For more information on Holographic Remoting in Unreal, see [here](https://docs.unrealengine.com/en-US/Platforms/AR/HoloLens/QuickStart/index.html).
1. The HoloLens 2 experience does not have UI to attach to a multiplayer experience as a client. A HoloLens 2 device runs as a listen server by default. Additional work is therefore needed to support multiple HoloLens 2 devices in the same multiplayer map.
1. Additional work is still needed to correctly draw content in the scene when a client joins a session late. The needed Azure Spatial Anchor identifiers to search for should already be replicated, but no logic exists to start a search for these anchors when a client joins late.

## Known Issues
1. iOS and Android devices may not support running as listen servers. Running the server experience for a mobile device will however allow the device to run a single user Azure Spatial Anchors experience. The more widely tested experience flow for this sample application is running a HoloLens 2 + Holographic Remoting in the editor as a listen server and attaching iOS and Android devices as clients.
1. Windows Mixed Reality blueprint functions specific to HoloLens 2 can cause compilation errors on Mac (Most of these functions are not defined for Mac). The workaround for this is to delete any Windows Mixed Reality blueprint nodes from the `BP_ARPawn` and to delete the `BP_MotionController` asset when building on Mac. This functionality will never be executed on iOS.