import 'package:flutter/material.dart';
import 'package:flutter_blue_plus/flutter_blue_plus.dart';
import 'package:permission_handler/permission_handler.dart';

void main() {
  runApp(const WindowCrankApp());
}

class WindowCrankApp extends StatelessWidget {
  const WindowCrankApp({super.key});

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'Window Crank Control',
      theme: ThemeData(primarySwatch: Colors.blue),
      home: const WindowCrankScreen(),
    );
  }
}

class WindowCrankScreen extends StatefulWidget {
  const WindowCrankScreen({super.key});

  @override
  State<WindowCrankScreen> createState() => _WindowCrankScreenState();
}

class _WindowCrankScreenState extends State<WindowCrankScreen> {
  BluetoothDevice? connectedDevice;
  BluetoothCharacteristic? positionChar;
  BluetoothCharacteristic? notificationChar;
  int currentPosition = 0;
  bool isConnected = false;
  
  final List<String> positions = ['Closed', 'Half', 'Open'];
  final TextEditingController messageController = TextEditingController();

  @override
  void initState() {
    super.initState();
    _requestPermissions();
  }

  Future<void> _requestPermissions() async {
    await Permission.bluetoothScan.request();
    await Permission.bluetoothConnect.request();
    await Permission.location.request();
  }

  Future<void> _scanAndConnect() async {
    FlutterBluePlus.startScan(timeout: const Duration(seconds: 10));
    
    FlutterBluePlus.scanResults.listen((results) async {
      for (ScanResult result in results) {
        if (result.device.platformName == 'WindowCrank') {
          FlutterBluePlus.stopScan();
          await _connectToDevice(result.device);
          break;
        }
      }
    });
  }

  Future<void> _connectToDevice(BluetoothDevice device) async {
    try {
      await device.connect();
      connectedDevice = device;
      
      List<BluetoothService> services = await device.discoverServices();
      for (BluetoothService service in services) {
        if (service.uuid.toString() == '12345678-1234-1234-1234-123456789abc') {
          for (BluetoothCharacteristic char in service.characteristics) {
            if (char.uuid.toString() == '87654321-4321-4321-4321-cba987654321') {
              positionChar = char;
            } else if (char.uuid.toString() == '11111111-2222-3333-4444-555555555555') {
              notificationChar = char;
            }
          }
        }
      }
      
      setState(() {
        isConnected = true;
      });
    } catch (e) {
      print('Connection failed: $e');
    }
  }

  Future<void> _sendPosition(int position) async {
    if (positionChar != null) {
      await positionChar!.write([position]);
      setState(() {
        currentPosition = position;
      });
    }
  }

  Future<void> _sendNotification() async {
    if (notificationChar != null && messageController.text.isNotEmpty) {
      String sender = 'Phone';
      String time = '${DateTime.now().hour.toString().padLeft(2, '0')}:${DateTime.now().minute.toString().padLeft(2, '0')}';
      String message = messageController.text;
      String data = '$sender|$time|$message';
      
      await notificationChar!.write(data.codeUnits);
      messageController.clear();
      
      ScaffoldMessenger.of(context).showSnackBar(
        const SnackBar(content: Text('Notification sent!')),
      );
    }
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: const Text('Window Crank Control'),
        backgroundColor: Colors.blue,
      ),
      body: Padding(
        padding: const EdgeInsets.all(16.0),
        child: Column(
          children: [
            // Connection Status
            Card(
              child: Padding(
                padding: const EdgeInsets.all(16.0),
                child: Row(
                  children: [
                    Icon(
                      isConnected ? Icons.bluetooth_connected : Icons.bluetooth_disabled,
                      color: isConnected ? Colors.green : Colors.red,
                    ),
                    const SizedBox(width: 8),
                    Text(isConnected ? 'Connected' : 'Disconnected'),
                    const Spacer(),
                    ElevatedButton(
                      onPressed: isConnected ? null : _scanAndConnect,
                      child: const Text('Connect'),
                    ),
                  ],
                ),
              ),
            ),
            
            const SizedBox(height: 20),
            
            // Current Position Display
            Card(
              child: Padding(
                padding: const EdgeInsets.all(16.0),
                child: Column(
                  children: [
                    const Text('Current Position', style: TextStyle(fontSize: 18, fontWeight: FontWeight.bold)),
                    const SizedBox(height: 10),
                    Text(
                      positions[currentPosition],
                      style: const TextStyle(fontSize: 24, color: Colors.blue),
                    ),
                  ],
                ),
              ),
            ),
            
            const SizedBox(height: 20),
            
            // Position Controls
            Card(
              child: Padding(
                padding: const EdgeInsets.all(16.0),
                child: Column(
                  children: [
                    const Text('Window Control', style: TextStyle(fontSize: 18, fontWeight: FontWeight.bold)),
                    const SizedBox(height: 10),
                    Row(
                      mainAxisAlignment: MainAxisAlignment.spaceEvenly,
                      children: [
                        ElevatedButton(
                          onPressed: isConnected ? () => _sendPosition(0) : null,
                          child: const Text('Closed'),
                        ),
                        ElevatedButton(
                          onPressed: isConnected ? () => _sendPosition(1) : null,
                          child: const Text('Half'),
                        ),
                        ElevatedButton(
                          onPressed: isConnected ? () => _sendPosition(2) : null,
                          child: const Text('Open'),
                        ),
                      ],
                    ),
                  ],
                ),
              ),
            ),
            
            const SizedBox(height: 20),
            
            // Notification Sender
            Card(
              child: Padding(
                padding: const EdgeInsets.all(16.0),
                child: Column(
                  children: [
                    const Text('Send Notification', style: TextStyle(fontSize: 18, fontWeight: FontWeight.bold)),
                    const SizedBox(height: 10),
                    TextField(
                      controller: messageController,
                      decoration: const InputDecoration(
                        hintText: 'Enter message...',
                        border: OutlineInputBorder(),
                      ),
                      maxLines: 3,
                    ),
                    const SizedBox(height: 10),
                    ElevatedButton(
                      onPressed: isConnected ? _sendNotification : null,
                      child: const Text('Send to Display'),
                    ),
                  ],
                ),
              ),
            ),
          ],
        ),
      ),
    );
  }
}