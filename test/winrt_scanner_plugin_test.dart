import 'package:flutter_test/flutter_test.dart';
import 'package:winrt_scanner_plugin/winrt_scanner_plugin.dart';
import 'package:winrt_scanner_plugin/winrt_scanner_plugin_platform_interface.dart';
import 'package:winrt_scanner_plugin/winrt_scanner_plugin_method_channel.dart';
import 'package:plugin_platform_interface/plugin_platform_interface.dart';

class MockWinrtScannerPluginPlatform
    with MockPlatformInterfaceMixin
    implements WinrtScannerPluginPlatform {
  @override
  Future<String?> getPlatformVersion() => Future.value('42');

  @override
  Future<List?> startScan(deviceId, source, colorMode, isDuplex, directory) {
    // TODO: implement startScan
    throw UnimplementedError();
  }

  @override
  Future<List?> getScanners() {
    // TODO: implement findAllScanners
    throw UnimplementedError();
  }
}

void main() {
  final WinrtScannerPluginPlatform initialPlatform =
      WinrtScannerPluginPlatform.instance;

  test('$MethodChannelWinrtScannerPlugin is the default instance', () {
    expect(initialPlatform, isInstanceOf<MethodChannelWinrtScannerPlugin>());
  });

  test('getPlatformVersion', () async {
    WinrtScannerPlugin winrtScannerPlugin = WinrtScannerPlugin();
    MockWinrtScannerPluginPlatform fakePlatform =
        MockWinrtScannerPluginPlatform();
    WinrtScannerPluginPlatform.instance = fakePlatform;

    expect(await winrtScannerPlugin.getPlatformVersion(), '42');
  });
}
