import 'package:plugin_platform_interface/plugin_platform_interface.dart';

import 'winrt_scanner_plugin_method_channel.dart';

abstract class WinrtScannerPluginPlatform extends PlatformInterface {
  /// Constructs a WinrtScannerPluginPlatform.
  WinrtScannerPluginPlatform() : super(token: _token);

  static final Object _token = Object();

  static WinrtScannerPluginPlatform _instance =
      MethodChannelWinrtScannerPlugin();

  /// The default instance of [WinrtScannerPluginPlatform] to use.
  ///
  /// Defaults to [MethodChannelWinrtScannerPlugin].
  static WinrtScannerPluginPlatform get instance => _instance;

  /// Platform-specific implementations should set this with their own
  /// platform-specific class that extends [WinrtScannerPluginPlatform] when
  /// they register themselves.
  static set instance(WinrtScannerPluginPlatform instance) {
    PlatformInterface.verifyToken(instance, _token);
    _instance = instance;
  }

  Future<String?> getPlatformVersion() {
    throw UnimplementedError('platformVersion() has not been implemented.');
  }

  Future<String?> startScan() {
    throw UnimplementedError('startScan() has not been implemented.');
  }
}
