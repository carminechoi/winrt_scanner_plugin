import 'package:flutter/services.dart';
import 'package:flutter_test/flutter_test.dart';
import 'package:winrt_scanner_plugin/winrt_scanner_plugin_method_channel.dart';

void main() {
  TestWidgetsFlutterBinding.ensureInitialized();

  MethodChannelWinrtScannerPlugin platform = MethodChannelWinrtScannerPlugin();
  const MethodChannel channel = MethodChannel('winrt_scanner_plugin');

  setUp(() {
    TestDefaultBinaryMessengerBinding.instance.defaultBinaryMessenger.setMockMethodCallHandler(
      channel,
      (MethodCall methodCall) async {
        return '42';
      },
    );
  });

  tearDown(() {
    TestDefaultBinaryMessengerBinding.instance.defaultBinaryMessenger.setMockMethodCallHandler(channel, null);
  });

  test('getPlatformVersion', () async {
    expect(await platform.getPlatformVersion(), '42');
  });
}
