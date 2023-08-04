import 'package:flutter/foundation.dart';
import 'package:flutter/services.dart';

import 'winrt_scanner_plugin_platform_interface.dart';

/// An implementation of [WinrtScannerPluginPlatform] that uses method channels.
class MethodChannelWinrtScannerPlugin extends WinrtScannerPluginPlatform {
  /// The method channel used to interact with the native platform.
  @visibleForTesting
  final methodChannel = const MethodChannel('winrt_scanner_plugin');

  @override
  Future<String?> getPlatformVersion() async {
    final version =
        await methodChannel.invokeMethod<String>('getPlatformVersion');
    return version;
  }

  @override
  Future<List?> getScanners() async {
    final scanners = await methodChannel.invokeMethod<List>('getScanners');
    print(scanners);
    return scanners;
  }

  @override
  Future<List?> startScan(
      deviceId, source, colorMode, isDuplex, directory) async {
    print('starting scan');
    try {
      final version = await methodChannel.invokeMethod<List>('startScan', {
        'deviceId': deviceId,
        'source': source,
        'colorMode': colorMode,
        'isDuplex': isDuplex,
        'directory': directory
      });
      return version;
    } catch (e) {
      print(e);
      return [];
    }
  }
}
