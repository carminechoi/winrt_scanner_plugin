import 'package:flutter/material.dart';
import 'package:path_provider/path_provider.dart';
import 'dart:async';

import 'package:flutter/services.dart';
import 'package:winrt_scanner_plugin/winrt_scanner_plugin.dart';

void main() {
  runApp(const MyApp());
}

class MyApp extends StatefulWidget {
  const MyApp({super.key});

  @override
  State<MyApp> createState() => _MyAppState();
}

class _MyAppState extends State<MyApp> {
  String _platformVersion = 'Unknown';
  List _scanners = [];
  final _winrtScannerPlugin = WinrtScannerPlugin();

  @override
  void initState() {
    super.initState();
    initPlatformState();
  }

  // Platform messages are asynchronous, so we initialize in an async method.
  Future<void> initPlatformState() async {
    String platformVersion;
    List scanners;
    // Platform messages may fail, so we use a try/catch PlatformException.
    // We also handle the message potentially returning null.
    try {
      platformVersion = await _winrtScannerPlugin.getPlatformVersion() ??
          'Unknown platform version';
    } on PlatformException {
      platformVersion = 'Failed to get platform version.';
    }

    try {
      scanners = await _winrtScannerPlugin.getScanners() ?? [];
    } catch (e) {
      scanners = [];
    }

    // If the widget was removed from the tree while the asynchronous platform
    // message was in flight, we want to discard the reply rather than calling
    // setState to update our non-existent appearance.
    if (!mounted) return;

    setState(() {
      _platformVersion = platformVersion;
      _scanners = scanners;
    });
  }

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      home: Scaffold(
        appBar: AppBar(
          title: const Text('Plugin example app'),
        ),
        body: Center(
            child: Column(
          children: [
            Text('Running on: $_platformVersion\n'),
            TextButton(
              onPressed: () {
                _winrtScannerPlugin.getScanners();
              },
              child: const Text("Test getScanners"),
            ),
            TextButton(
              onPressed: () async {
                String deviceId = _scanners.first["deviceId"];
                String source = "Feeder";
                String colorMode = "Color";
                bool isDuplex = false;
                var directory = await getApplicationDocumentsDirectory();

                var scannedFiles = await _winrtScannerPlugin.startScan(
                  deviceId,
                  source,
                  colorMode,
                  isDuplex,
                  directory.path,
                );
                print(scannedFiles);
              },
              child: const Text("Test startScan"),
            )
          ],
        )),
      ),
    );
  }
}
