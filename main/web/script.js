// DualKey 控制面板 JavaScript 功能
// TODO: 当有设备传入connected false的时候，会直接删除容器第一个设备。暂时废弃不使用connected

// 语言切换功能
class LanguageManager {
    constructor() {
        this.currentLanguage = localStorage.getItem('language') || 'zh';
        this.translations = {
            zh: {
                title: 'DualKey 控制面板 v0.8',
                connectionStatus: '连接状态:',
                lastUpdate: '最后更新:',
                online: '在线',
                offline: '离线',
                dualkeyProperties: 'DualKey 属性',
                basicInfo: '基础信息',
                leftKey: '左键',
                rightKey: '右键',
                leftKeyColor: '左键颜色',
                rightKeyColor: '右键颜色',
                dipSwitchPosition: '拨码开关位置',
                left: '左',
                center: '中',
                right: '右',
                batteryStatus: '电量状态',
                batteryVoltage: '电池电压:',
                usbVoltage: 'USB电压:',
                switch1Value: '拨码开关1值:',
                switch2Value: '拨码开关2值:',
                charging: '充电中',
                fullyCharged: '已充满',
                notCharging: '未充电',
                hidDeviceStatus: 'HID 设备状态',
                usbConnection: 'USB连接:',
                bluetoothConnection: '蓝牙连接:',
                deviceName: '设备名称:',
                deviceType: '设备类型:',
                keyMapping: '按键映射:',
                mappingSwitch: '映射开关:',
                usbMapping: 'USB映射',
                bluetoothMapping: '蓝牙映射',
                currentMappingStatus: '当前映射状态:',
                startBroadcast: '开始广播',
                stopBroadcast: '关闭广播',
                pairingMode: '配对模式',
                wifiInfo: 'WIFI信息',
                wifiConfig: 'WIFI配置(双键长按重置)',
                ssid: 'SSID:',
                ipAddress: 'IP地址:',
                signalStrength: '信号强度:',
                wifiStatus: '连接状态:',
                password: '密码:',
                useStaticIP: '使用静态IP',
                ipAddress: 'IP地址:',
                subnetMask: '子网掩码:',
                gateway: '网关:',
                applyConfig: '应用配置',
                resetConfig: '重置配置',
                chainBusProperties: 'Chain Bus 属性',
                leftBus: '左 Bus',
                rightBus: '右 Bus',
                deviceCount: '设备数量:',
                refreshTime: '刷新时间:',
                deviceList: '设备列表',
                busRGB: '总线RGB:',
                set: '设置',
                refreshBusEnumeration: '刷新BUS枚举设备',
                connected: '已连接',
                notConnected: '未连接',
                disconnected: '已断开',
                copyPaste: '复制粘贴 (Ctrl+C/V)',
                copyPasteCmd: '复制粘贴 (Cmd+C/V)',
                undoRedo: '撤销重做 (Ctrl+Z/Y)',
                undoRedoCmd: '撤销重做 (Cmd+Z/Y)',
                undoRedoCmdShift: '撤销重做 (Cmd+Z / Cmd+Shift+Z)',
                tabSwitch: '标签页 (Ctrl+Tab / Ctrl+Shift+Tab)',
                windowSwitch: '窗口切换 (Alt/Tab)',
                windowSwitchCmd: '窗口切换 (Cmd/Tab)',
                zoom: '缩放 (Ctrl+Minus/Plus)',
                zoomCmd: '缩放 (Cmd+Minus/Plus)',
                pageUpDown: '翻页 (Page Up/Down)',
                volumeControl: '音量控制 (Vol+/-)',
                mediaControl: '媒体控制 (Prev/Next)',
                mediaControlPlayPause: '媒体控制 (PlayPause/Stop)',
                homeEnd: 'HOME按键 (Home/End)',
                upDown: '方向键 (Up/Down)',
                leftRight: '方向键 (Left/Right)',
                apply: '应用',
                enabled: '已启用',
                disabled: '已禁用',
                // 自定义映射
                presetMapping: '预设组合',
                customKeyMapping: '自定义按键',
                customTextMapping: '自定义文本',
                customTextNoticeText: '仅支持 ASCII 可打印字符，暂不支持中文等需编码文字',
                leftKeyText: '左键文本:',
                rightKeyText: '右键文本:',
                noKey: '-- 无 --',
                // HID功能选项
                noFunction: '无功能',
                copy: '复制',
                paste: '粘贴',
                cut: '剪切',
                undo: '撤销',
                redo: '重做',
                selectAll: '全选',
                delete: '删除',
                backspace: '退格',
                enter: '回车',
                escape: 'Esc',
                tab: 'Tab',
                pageUp: '上翻页',
                pageDown: '下翻页',
                mouseLeftPress: '鼠标左键按下',
                mouseLeftRelease: '鼠标左键释放',
                mouseRightPress: '鼠标右键按下',
                mouseRightRelease: '鼠标右键释放',
                mouseMove: '鼠标移动',
                mouseMoveLeft: '鼠标向左移动',
                mouseMoveRight: '鼠标向右移动',
                mouseMoveUp: '鼠标向上移动',
                mouseMoveDown: '鼠标向下移动',
                mouseScrollVertical: '鼠标竖向滚轮',
                mouseScrollUp: '鼠标竖向滚轮向上',
                mouseScrollDown: '鼠标竖向滚轮向下',
                mouseScrollHorizontal: '鼠标横向滚轮',
                mouseScrollLeft: '鼠标横向滚轮向左',
                mouseScrollRight: '鼠标横向滚轮向右',
                volumeUp: '音量增加',
                volumeDown: '音量减少',
                muteToggle: '静音切换',
                playPause: '播放暂停',
                nextTrack: '下一首',
                prevTrack: '上一首',
                stopPlay: '停止播放',
                // 设备状态
                pressed: '按下',
                released: '释放',
                someoneDetected: '有人',
                noOneDetected: '无人',
                switchOn: '打开',
                switchOff: '关闭',
                noData: '暂无数据',
                // 设备数据标签
                buttonStatus: '按键状态',
                detectStatus: '检测状态',
                triggerCount: '触发次数',
                xAxis: 'X轴',
                yAxis: 'Y轴',
                button: '按键',
                encoderValue: '编码器值',
                distance: '距离',
                angleADC: '角度ADC',
                switchStatus: '开关状态',
                switchCount: '切换次数',
                // UI状态文本
                dataRefresh: '数据刷新',
                pauseRefresh: '暂停刷新',
                rgbSetting: 'RGB设置中...',
                setting: '设置中...',
                configuring: '配置中...',
                applying: '应用中...',
                stopping: '停止中...',
                starting: '启动中...',
                pairing: '配对中...',
                restoring: '恢复中...',
                restoreDefault: '恢复默认',
                noDevices: '暂无设备',
                // 按钮和标签
                pressFunction: '按下功能',
                releaseFunction: '释放功能',
                singleClickFunction: '单击功能',
                doubleClickFunction: '双击功能',
                longPressFunction: '长按功能',
                xyMoveFunction: 'XY轴移动功能',
                directionReverse: '方向翻转',
                rotateClockwiseFunction: '顺时针旋转功能',
                rotateCounterClockwiseFunction: '逆时针旋转功能',
                angleChangeFunction: '角度变化功能',
                hidFunctionConfig: 'HID功能配置',
                applyConfig: '应用配置',
                resetConfig: '重置配置',
                updateTime: '更新时间:',
                rgbColor: 'RGB颜色',
                event: '事件:',
                // 确认对话框
                confirmResetHID: '确定要恢复设备 {deviceId} 的HID配置为默认值吗？这将清除所有已保存的配置。',
                confirmResetWifi: '确定要重置WiFi配置吗？设备将重启并开启AP热点模式。',
                enterWifiSSID: '请输入WiFi SSID',
                enterWifiPassword: '请输入WiFi密码',
                enterStaticIPConfig: '请填写完整的静态IP配置',
                wifiConfigSaved: 'WiFi配置已保存，设备将重启并尝试连接...',
                // 设备类型显示名称
                chainKey: 'Chain Key',
                chainJoystick: 'Chain Joystick',
                chainEncoder: 'Chain Encoder',
                chainAngle: 'Chain Angle',
                chainToF: 'Chain ToF',
                chainPIR: 'Chain PIR',
                chainSwitch: 'Chain Switch',
                chainUART: 'Chain UART',
                chainPedal: 'Chain Pedal',
                chainMIC: 'Chain MIC',
                chainBuzzer: 'Chain Buzzer',
                chainServos: 'Unit 8Servos2Chain',
                chainMono: 'Chain Mono',
                chainRGB: 'Chain RGB',
                chainENV: 'Chain ENV',
                chainIMU: 'Chain IMU',
                chainDLight: 'Chain DLight',
                chainBusUnit: 'Unit ChainBus',
                temperature: '温度',
                humidity: '湿度',
                pressure: '气压',
                altitude: '海拔',
                lux: '亮度',
                accel: '加速度',
                gyro: '陀螺仪',
                servoAngle: '舵机角度',
                dcVoltage: 'DC电压',
                groveVoltage: 'Grove电压',
                i2cAddrs: 'I2C地址',
                scanI2C: '扫描I2C',
                clearScreen: '清屏',
                scrollText: '滚动文字',
                scrollTextPlaceholder: '输入滚动文字',
                playNote: '播放音符',
                frequency: '频率(Hz)',
                duration: '时长(ms)',
                playTone: '播放',
                drawPixel: '点击绘点',
                servoChannel: '通道',
                interactiveControl: '交互控制',
                arrowKeys: '方向键',
                mapWASD: '映射 WASD',
                mapArrows: '映射方向键',
                buzzerSingleNote: '单音符',
                buzzerCustomTone: '自定义音调',
                buzzerMelody: '内置音效',
                dutyCycle: '占空比(%)',
                stop: '停止',
                loop: '循环',
                playEffect: '播放音效',
                pixelDraw: '点阵',
                scroll: '滚动',
                direction: '方向',
                scrollMode: '模式',
                scrollOnce: '一次',
                scrollLoop: '循环',
                scrollPingPong: '往返',
                intervalMs: '间隔(ms)',
                scrollColor: '颜色',
                syncBusMono: '同步本总线所有 MONO',
                syncBusRgb: '同步本总线所有 RGB',
                highThreshold: '高阈值',
                lowThreshold: '低阈值',
                luxTooHigh: '过高触发',
                luxTooLow: '过低触发',
                triggerInterval: '触发间隔 (ms)',
                micThreshold: '阈值 (ADC)',
                aboveThreshold: '超过阈值',
                belowThreshold: '低于阈值',
                switchOnAction: '开关打开',
                switchOffAction: '开关关闭',
                personEnter: '人体进入',
                personLeave: '人体离开',
                scrollDirLeft: '左',
                scrollDirRight: '右',
                scrollDirUp: '上',
                scrollDirDown: '下',
                presetMario: 'Mario',
                presetDoorbell: 'Doorbell',
                presetScale: 'Scale',
                presetAlarm: 'Alarm',
                unknownFunction: '未知功能',
                // 配对提示
                pairingTip: '目前仅支持旧主机删除连接后才能与新设备配对',
            },
            en: {
                title: 'DualKey Control Panel v0.8',
                connectionStatus: 'Connection Status:',
                lastUpdate: 'Last Update:',
                online: 'Online',
                offline: 'Offline',
                dualkeyProperties: 'DualKey Properties',
                basicInfo: 'Basic Information',
                leftKey: 'Left Key',
                rightKey: 'Right Key',
                leftKeyColor: 'Left Key Color',
                rightKeyColor: 'Right Key Color',
                dipSwitchPosition: 'DIP Switch Position',
                left: 'Left',
                center: 'Center',
                right: 'Right',
                batteryStatus: 'Battery Status',
                batteryVoltage: 'Battery Voltage:',
                usbVoltage: 'USB Voltage:',
                charging: 'Charging',
                switch1Value: 'Switch 1 Value:',
                switch2Value: 'Switch 2 Value:',
                fullyCharged: 'Fully Charged',
                notCharging: 'Not Charging',
                hidDeviceStatus: 'HID Device Status',
                usbConnection: 'USB Connection:',
                bluetoothConnection: 'Bluetooth Connection:',
                deviceName: 'Device Name:',
                deviceType: 'Device Type:',
                keyMapping: 'Key Mapping:',
                mappingSwitch: 'Mapping Switch:',
                usbMapping: 'USB Mapping',
                bluetoothMapping: 'Bluetooth Mapping',
                currentMappingStatus: 'Current Mapping Status:',
                startBroadcast: 'Start Broadcast',
                stopBroadcast: 'Stop Broadcast',
                pairingMode: 'Pairing Mode',
                wifiInfo: 'WIFI Information',
                wifiConfig: 'WIFI Configuration (Long press both keys to reset)',
                ssid: 'SSID:',
                ipAddress: 'IP Address:',
                signalStrength: 'Signal Strength:',
                wifiStatus: 'Connection Status:',
                password: 'Password:',
                useStaticIP: 'Use Static IP',
                ipAddress: 'IP Address:',
                subnetMask: 'Subnet Mask:',
                gateway: 'Gateway:',
                applyConfig: 'Apply Configuration',
                resetConfig: 'Reset Configuration',
                chainBusProperties: 'Chain Bus Properties',
                leftBus: 'Left Bus',
                rightBus: 'Right Bus',
                deviceCount: 'Device Count:',
                refreshTime: 'Refresh Time:',
                deviceList: 'Device List',
                busRGB: 'Bus RGB:',
                set: 'Set',
                refreshBusEnumeration: 'Refresh BUS Enumeration',
                connected: 'Connected',
                notConnected: 'Not Connected',
                disconnected: 'Disconnected',
                copyPaste: 'Copy Paste (Ctrl+C/V)',
                copyPasteCmd: 'Copy Paste (Cmd+C/V)',
                undoRedo: 'Undo Redo (Ctrl+Z/Y)',
                undoRedoCmd: 'Undo Redo (Cmd+Z/Y)',
                undoRedoCmdShift: 'Undo Redo (Cmd+Z / Cmd+Shift+Z)',
                tabSwitch: 'Tab Switch (Ctrl+Tab / Ctrl+Shift+Tab)',
                windowSwitchCmd: 'Window Switch (Cmd/Tab)',
                windowSwitch: 'Window Switch (Alt/Tab)',
                zoom: 'Zoom (Ctrl+Minus/Plus)',
                zoomCmd: 'Zoom (Cmd+Minus/Plus)',
                pageUpDown: 'Page Up/Down',
                volumeControl: 'Volume Control (Vol+/-)',
                mediaControl: 'Media Control (Prev/Next)',
                mediaControlPlayPause: 'Media Control (PlayPause/Stop)',
                homeEnd: 'HOME Key (Home/End)',
                upDown: 'Arrow Keys (Up/Down)',
                leftRight: 'Arrow Keys (Left/Right)',
                apply: 'Apply',
                enabled: 'Enabled',
                disabled: 'Disabled',
                // Custom mapping
                presetMapping: 'Preset Combination',
                customKeyMapping: 'Custom Key',
                customTextMapping: 'Custom Text',
                customTextNoticeText: 'Only ASCII printable characters are supported. Chinese and other encoded characters are not supported.',
                leftKeyText: 'Left Key Text:',
                rightKeyText: 'Right Key Text:',
                noKey: '-- None --',
                // HID功能选项
                noFunction: 'No Function',
                copy: 'Copy',
                paste: 'Paste',
                cut: 'Cut',
                undo: 'Undo',
                redo: 'Redo',
                selectAll: 'Select All',
                delete: 'Delete',
                backspace: 'Backspace',
                enter: 'Enter',
                escape: 'Esc',
                tab: 'Tab',
                pageUp: 'Page Up',
                pageDown: 'Page Down',
                mouseLeftPress: 'Mouse Left Press',
                mouseLeftRelease: 'Mouse Left Release',
                mouseRightPress: 'Mouse Right Press',
                mouseRightRelease: 'Mouse Right Release',
                mouseMove: 'Mouse Move',
                mouseMoveLeft: 'Mouse Move Left',
                mouseMoveRight: 'Mouse Move Right',
                mouseMoveUp: 'Mouse Move Up',
                mouseMoveDown: 'Mouse Move Down',
                mouseScrollVertical: 'Mouse Scroll Vertical',
                mouseScrollUp: 'Mouse Scroll Up',
                mouseScrollDown: 'Mouse Scroll Down',
                mouseScrollHorizontal: 'Mouse Scroll Horizontal',
                mouseScrollLeft: 'Mouse Scroll Left',
                mouseScrollRight: 'Mouse Scroll Right',
                volumeUp: 'Volume Up',
                volumeDown: 'Volume Down',
                muteToggle: 'Mute Toggle',
                playPause: 'Play/Pause',
                nextTrack: 'Next Track',
                prevTrack: 'Previous Track',
                stopPlay: 'Stop Play',
                // 设备状态
                pressed: 'Pressed',
                released: 'Released',
                someoneDetected: 'Someone Detected',
                noOneDetected: 'No One Detected',
                switchOn: 'On',
                switchOff: 'Off',
                noData: 'No Data',
                // 设备数据标签
                buttonStatus: 'Button Status',
                detectStatus: 'Detection Status',
                triggerCount: 'Trigger Count',
                xAxis: 'X Axis',
                yAxis: 'Y Axis',
                button: 'Button',
                encoderValue: 'Encoder Value',
                distance: 'Distance',
                angleADC: 'Angle ADC',
                switchStatus: 'Switch Status',
                switchCount: 'Switch Count',
                // UI状态文本
                dataRefresh: 'Data Refresh',
                pauseRefresh: 'Pause Refresh',
                rgbSetting: 'RGB Setting...',
                setting: 'Setting...',
                configuring: 'Configuring...',
                applying: 'Applying...',
                stopping: 'Stopping...',
                starting: 'Starting...',
                pairing: 'Pairing...',
                restoring: 'Restoring...',
                restoreDefault: 'Restore Default',
                noDevices: 'No Devices',
                // 按钮和标签
                pressFunction: 'Press Function',
                releaseFunction: 'Release Function',
                singleClickFunction: 'Single Click Function',
                doubleClickFunction: 'Double Click Function',
                longPressFunction: 'Long Press Function',
                xyMoveFunction: 'XY Move Function',
                directionReverse: 'Direction Reverse',
                rotateClockwiseFunction: 'Rotate Clockwise Function',
                rotateCounterClockwiseFunction: 'Rotate Counter-Clockwise Function',
                angleChangeFunction: 'Angle Change Function',
                hidFunctionConfig: 'HID Function Config',
                applyConfig: 'Apply Config',
                resetConfig: 'Reset Config',
                rgbColor: 'RGB Color',
                updateTime: 'Update Time:',
                event: 'Event:',
                // 确认对话框
                confirmResetHID: 'Are you sure you want to restore device {deviceId} HID configuration to default values? This will clear all saved configurations.',
                confirmResetWifi: 'Are you sure you want to reset WiFi configuration? The device will restart and enable AP hotspot mode.',
                enterWifiSSID: 'Please enter WiFi SSID',
                enterWifiPassword: 'Please enter WiFi password',
                enterStaticIPConfig: 'Please fill in complete static IP configuration',
                wifiConfigSaved: 'WiFi configuration saved, device will restart and try to connect...',
                // 设备类型显示名称
                chainKey: 'Chain Key',
                chainJoystick: 'Chain Joystick',
                chainEncoder: 'Chain Encoder',
                chainAngle: 'Chain Angle',
                chainToF: 'Chain ToF',
                chainPIR: 'Chain PIR',
                chainSwitch: 'Chain Switch',
                chainUART: 'Chain UART',
                chainPedal: 'Chain Pedal',
                chainMIC: 'Chain MIC',
                chainBuzzer: 'Chain Buzzer',
                chainServos: 'Unit 8Servos2Chain',
                chainMono: 'Chain Mono',
                chainRGB: 'Chain RGB',
                chainENV: 'Chain ENV',
                chainIMU: 'Chain IMU',
                chainDLight: 'Chain DLight',
                chainBusUnit: 'Unit ChainBus',
                temperature: 'Temperature',
                humidity: 'Humidity',
                pressure: 'Pressure',
                altitude: 'Altitude',
                lux: 'Brightness',
                accel: 'Accel',
                gyro: 'Gyro',
                servoAngle: 'Servo Angle',
                dcVoltage: 'DC Voltage',
                groveVoltage: 'Grove Voltage',
                i2cAddrs: 'I2C Addresses',
                scanI2C: 'Scan I2C',
                clearScreen: 'Clear',
                scrollText: 'Scroll Text',
                scrollTextPlaceholder: 'Enter scroll text',
                playNote: 'Play Note',
                frequency: 'Frequency (Hz)',
                duration: 'Duration (ms)',
                playTone: 'Play',
                drawPixel: 'Click to draw',
                servoChannel: 'Channel',
                interactiveControl: 'Interactive Control',
                arrowKeys: 'Arrow Keys',
                mapWASD: 'Map WASD',
                mapArrows: 'Map Arrow Keys',
                buzzerSingleNote: 'Single Note',
                buzzerCustomTone: 'Custom Tone',
                buzzerMelody: 'Sound Effects',
                dutyCycle: 'Duty (%)',
                stop: 'Stop',
                loop: 'Loop',
                playEffect: 'Play Effect',
                pixelDraw: 'Pixel',
                scroll: 'Scroll',
                direction: 'Direction',
                scrollMode: 'Mode',
                scrollOnce: 'Once',
                scrollLoop: 'Loop',
                scrollPingPong: 'Ping-Pong',
                intervalMs: 'Interval (ms)',
                scrollColor: 'Color',
                syncBusMono: 'Sync all MONO on bus',
                syncBusRgb: 'Sync all RGB on bus',
                highThreshold: 'High Threshold',
                lowThreshold: 'Low Threshold',
                luxTooHigh: 'Too Bright Action',
                luxTooLow: 'Too Dark Action',
                triggerInterval: 'Trigger Interval (ms)',
                micThreshold: 'Threshold (ADC)',
                aboveThreshold: 'Above Threshold',
                belowThreshold: 'Below Threshold',
                switchOnAction: 'Switch On',
                switchOffAction: 'Switch Off',
                personEnter: 'Person Detected',
                personLeave: 'Person Left',
                scrollDirLeft: 'Left',
                scrollDirRight: 'Right',
                scrollDirUp: 'Up',
                scrollDirDown: 'Down',
                presetMario: 'Mario',
                presetDoorbell: 'Doorbell',
                presetScale: 'Scale',
                presetAlarm: 'Alarm',
                unknownFunction: 'Unknown Function',
                // 配对提示
                pairingTip: 'Currently only supports pairing with new devices after the old host deletes the connection',
            }
        };
        this.init();
    }

    init() {
        this.updateLanguage();
        this.setupLanguageToggle();
    }

    setupLanguageToggle() {
        const toggleBtn = document.getElementById('languageToggle');
        if (toggleBtn) {
            toggleBtn.addEventListener('click', () => {
                this.toggleLanguage();
            });
        }
    }

    toggleLanguage() {
        this.currentLanguage = this.currentLanguage === 'zh' ? 'en' : 'zh';
        localStorage.setItem('language', this.currentLanguage);
        this.updateLanguage();
    }

    updateLanguage() {
        // 更新语言切换按钮文本
        const langText = document.querySelector('.lang-text');
        if (langText) {
            langText.textContent = this.currentLanguage === 'zh' ? 'EN' : '中';
        }

        // 更新页面标题
        document.title = this.translations[this.currentLanguage].title;

        // 更新HTML lang属性
        document.documentElement.lang = this.currentLanguage === 'zh' ? 'zh-CN' : 'en';

        // 更新所有带有data-i18n属性的元素
        const elements = document.querySelectorAll('[data-i18n]');
        elements.forEach(element => {
            const key = element.getAttribute('data-i18n');
            if (this.translations[this.currentLanguage][key]) {
                element.textContent = this.translations[this.currentLanguage][key];
            }
        });

        // 更新所有带有data-i18n-title属性的元素的title属性
        const titleElements = document.querySelectorAll('[data-i18n-title]');
        titleElements.forEach(element => {
            const key = element.getAttribute('data-i18n-title');
            if (this.translations[this.currentLanguage][key]) {
                element.title = this.translations[this.currentLanguage][key];
            }
        });

        // 更新placeholder文本
        this.updatePlaceholders();
        
        // 通知DualKeyController刷新设备容器
        if (window.dualkeyController) {
            window.dualkeyController.refreshDeviceContainers();
        }
    }

    updatePlaceholders() {
        const placeholders = {
            zh: {
                'wifiConfigSSID': '输入WiFi名称',
                'wifiConfigPassword': '输入WiFi密码',
                'wifiStaticIP': '192.168.1.100',
                'wifiStaticNetmask': '255.255.255.0',
                'wifiStaticGateway': '192.168.1.1',
                'leftKeyText': '按下左键时输出的文本...',
                'rightKeyText': '按下右键时输出的文本...'
            },
            en: {
                'wifiConfigSSID': 'Enter WiFi Name',
                'wifiConfigPassword': 'Enter WiFi Password',
                'wifiStaticIP': '192.168.1.100',
                'wifiStaticNetmask': '255.255.255.0',
                'wifiStaticGateway': '192.168.1.1',
                'leftKeyText': 'Text to type when left key is pressed...',
                'rightKeyText': 'Text to type when right key is pressed...'
            }
        };

        Object.keys(placeholders[this.currentLanguage]).forEach(id => {
            const element = document.getElementById(id);
            if (element) {
                element.placeholder = placeholders[this.currentLanguage][id];
            }
        });
    }

    getText(key) {
        return this.translations[this.currentLanguage][key] || key;
    }
}

// 初始化语言管理器
const languageManager = new LanguageManager();

class DualKeyController {
    constructor() {
        this.websocket = null;
        this.reconnectAttempts = 0;
        this.reconnectDelay = 1000;
        
        // 添加心跳检测相关变量
        this.heartbeatInterval = null;
        this.lastDataTime = Date.now();
        this.offlineTimeout = 5000; // 5秒没有数据就显示离线
        this.heartbeatCheckInterval = 1000; // 每秒检查一次心跳
        
        console.log(`心跳检测配置: 离线超时=${this.offlineTimeout}ms, 检查间隔=${this.heartbeatCheckInterval}ms`);

        // 添加HID状态缓存，避免频繁DOM更新
        this.hidStatusCache = {
            usbConnected: false,
            bluetoothConnected: false,
            bluetoothAdvStatus: false,
            deviceName: "Chain DualKey",
            deviceType: "HID Keyboard",
            currentKeyMapping: 9,
            bluetoothPairingStatus: 0,
            usbMappingEnabled: true,
            bleMappingEnabled: true,
            customMappingEnabled: false,
            customLeftAction: { action_type: 0, modifier: 0, keycode: 0x4B, text: '' },
            customRightAction: { action_type: 0, modifier: 0, keycode: 0x4E, text: '' }
        };

        // 当前映射标签: 'preset' | 'customKey' | 'customText'
        this.currentMappingTab = 'preset';

        // HID 键码选项（key: HID usage ID, label: 显示名称）
        this.keyCodeOptions = [
            { value: 0,   label: '-- None --' },
            // 字母
            { value: 4,   label: 'A' }, { value: 5,   label: 'B' }, { value: 6,   label: 'C' },
            { value: 7,   label: 'D' }, { value: 8,   label: 'E' }, { value: 9,   label: 'F' },
            { value: 10,  label: 'G' }, { value: 11,  label: 'H' }, { value: 12,  label: 'I' },
            { value: 13,  label: 'J' }, { value: 14,  label: 'K' }, { value: 15,  label: 'L' },
            { value: 16,  label: 'M' }, { value: 17,  label: 'N' }, { value: 18,  label: 'O' },
            { value: 19,  label: 'P' }, { value: 20,  label: 'Q' }, { value: 21,  label: 'R' },
            { value: 22,  label: 'S' }, { value: 23,  label: 'T' }, { value: 24,  label: 'U' },
            { value: 25,  label: 'V' }, { value: 26,  label: 'W' }, { value: 27,  label: 'X' },
            { value: 28,  label: 'Y' }, { value: 29,  label: 'Z' },
            // 数字
            { value: 39,  label: '0' }, { value: 30,  label: '1' }, { value: 31,  label: '2' },
            { value: 32,  label: '3' }, { value: 33,  label: '4' }, { value: 34,  label: '5' },
            { value: 35,  label: '6' }, { value: 36,  label: '7' }, { value: 37,  label: '8' },
            { value: 38,  label: '9' },
            // F 键
            { value: 58,  label: 'F1'  }, { value: 59,  label: 'F2'  }, { value: 60,  label: 'F3'  },
            { value: 61,  label: 'F4'  }, { value: 62,  label: 'F5'  }, { value: 63,  label: 'F6'  },
            { value: 64,  label: 'F7'  }, { value: 65,  label: 'F8'  }, { value: 66,  label: 'F9'  },
            { value: 67,  label: 'F10' }, { value: 68,  label: 'F11' }, { value: 69,  label: 'F12' },
            // 特殊键
            { value: 40,  label: 'Enter'       },
            { value: 41,  label: 'Escape'      },
            { value: 42,  label: 'Backspace'   },
            { value: 43,  label: 'Tab'         },
            { value: 44,  label: 'Space'       },
            { value: 76,  label: 'Delete'      },
            { value: 73,  label: 'Insert'      },
            { value: 57,  label: 'Caps Lock'   },
            // 导航键
            { value: 74,  label: 'Home'        },
            { value: 77,  label: 'End'         },
            { value: 75,  label: 'Page Up'     },
            { value: 78,  label: 'Page Down'   },
            { value: 82,  label: '↑ Up'        },
            { value: 81,  label: '↓ Down'      },
            { value: 80,  label: '← Left'      },
            { value: 79,  label: '→ Right'     },
            // 其他
            { value: 70,  label: 'Print Screen' },
            { value: 71,  label: 'Scroll Lock'  },
            { value: 72,  label: 'Pause'         },
        ];
        
        this.init();
        
        // 等待DOM加载完成后再绑定事件
        if (document.readyState === 'loading') {
            document.addEventListener('DOMContentLoaded', () => {
                this.initStatusElements();
                this.setupEventListeners();
                this.connectWebSocket();
                this.startHeartbeatCheck();
            });
        } else {
            // DOM已经加载完成
            this.initStatusElements();
            this.setupEventListeners();
            this.connectWebSocket();
            this.startHeartbeatCheck();
        }

        // 修改设备缓存结构 - 使用设备ID作为key
        this.deviceCache = {
            left: new Map(),    // key: deviceId, value: device对象
            right: new Map()    // key: deviceId, value: device对象
        };

        // 修改事件定时器缓存结构 - 使用设备ID作为key
        this.eventTimers = {
            left: new Map(),    // key: deviceId, value: timerId
            right: new Map()    // key: deviceId, value: timerId
        };

        // 修改设备数据刷新开关状态结构 - 使用设备ID作为key
        this.deviceRefreshEnabled = {
            left: new Map(),
            right: new Map()
        };
        this.panelState = new Map();

        this.initHIDFunctionOptions();
    }

    initHIDFunctionOptions() {
        // HID功能选项硬编码数据（对应chain_bus_hid.h中的enum）
        this.hidFunctionOptions = [
            {value: 0, name: languageManager.getText('noFunction'), desc: languageManager.getText('noFunction')},
            {value: 1, name: languageManager.getText('copy'), desc: "Ctrl+C " + languageManager.getText('copy')},
            {value: 2, name: languageManager.getText('paste'), desc: "Ctrl+V " + languageManager.getText('paste')},
            {value: 3, name: languageManager.getText('cut'), desc: "Ctrl+X " + languageManager.getText('cut')},
            {value: 4, name: languageManager.getText('undo'), desc: "Ctrl+Z " + languageManager.getText('undo')},
            {value: 5, name: languageManager.getText('redo'), desc: "Ctrl+Y " + languageManager.getText('redo')},
            {value: 6, name: languageManager.getText('selectAll'), desc: "Ctrl+A " + languageManager.getText('selectAll')},
            {value: 7, name: languageManager.getText('delete'), desc: "Delete " + languageManager.getText('delete')},
            {value: 8, name: languageManager.getText('backspace'), desc: "Backspace " + languageManager.getText('backspace')},
            {value: 9, name: languageManager.getText('enter'), desc: "Enter " + languageManager.getText('enter')},
            {value: 10, name: languageManager.getText('escape'), desc: languageManager.getText('escape')},
            {value: 11, name: languageManager.getText('tab'), desc: languageManager.getText('tab')},
            {value: 12, name: languageManager.getText('pageUp'), desc: "Page Up"},
            {value: 13, name: languageManager.getText('pageDown'), desc: "Page Down"},
            {value: 14, name: languageManager.getText('mouseLeftPress'), desc: languageManager.getText('mouseLeftPress')},
            {value: 15, name: languageManager.getText('mouseLeftRelease'), desc: languageManager.getText('mouseLeftRelease')},
            {value: 16, name: languageManager.getText('mouseRightPress'), desc: languageManager.getText('mouseRightPress')},
            {value: 17, name: languageManager.getText('mouseRightRelease'), desc: languageManager.getText('mouseRightRelease')},
            {value: 18, name: languageManager.getText('mouseMove'), desc: languageManager.getText('mouseMove')},
            {value: 19, name: languageManager.getText('mouseMoveLeft'), desc: languageManager.getText('mouseMoveLeft')},
            {value: 20, name: languageManager.getText('mouseMoveRight'), desc: languageManager.getText('mouseMoveRight')},
            {value: 21, name: languageManager.getText('mouseMoveUp'), desc: languageManager.getText('mouseMoveUp')},
            {value: 22, name: languageManager.getText('mouseMoveDown'), desc: languageManager.getText('mouseMoveDown')},
            {value: 23, name: languageManager.getText('mouseScrollVertical'), desc: languageManager.getText('mouseScrollVertical')},
            {value: 24, name: languageManager.getText('mouseScrollUp'), desc: languageManager.getText('mouseScrollUp')},
            {value: 25, name: languageManager.getText('mouseScrollDown'), desc: languageManager.getText('mouseScrollDown')},
            {value: 26, name: languageManager.getText('mouseScrollHorizontal'), desc: languageManager.getText('mouseScrollHorizontal')},
            {value: 27, name: languageManager.getText('mouseScrollLeft'), desc: languageManager.getText('mouseScrollLeft')},
            {value: 28, name: languageManager.getText('mouseScrollRight'), desc: languageManager.getText('mouseScrollRight')},
            {value: 29, name: languageManager.getText('volumeUp'), desc: languageManager.getText('volumeUp')},
            {value: 30, name: languageManager.getText('volumeDown'), desc: languageManager.getText('volumeDown')},
            {value: 31, name: languageManager.getText('muteToggle'), desc: languageManager.getText('muteToggle')},
            {value: 32, name: languageManager.getText('playPause'), desc: languageManager.getText('playPause')},
            {value: 33, name: languageManager.getText('nextTrack'), desc: languageManager.getText('nextTrack')},
            {value: 34, name: languageManager.getText('prevTrack'), desc: languageManager.getText('prevTrack')},
            {value: 35, name: languageManager.getText('stopPlay'), desc: languageManager.getText('stopPlay')},
            {value: 36, name: languageManager.getText('mapWASD'), desc: languageManager.getText('mapWASD')},
            {value: 37, name: languageManager.getText('arrowKeys'), desc: languageManager.getText('mapArrows')}
        ];

        this.KeyHIDFunctionOptions = [
            {value: 0, name: languageManager.getText('noFunction'), desc: languageManager.getText('noFunction')},
            {value: 1, name: languageManager.getText('copy'), desc: "Ctrl+C " + languageManager.getText('copy')},
            {value: 2, name: languageManager.getText('paste'), desc: "Ctrl+V " + languageManager.getText('paste')},
            {value: 3, name: languageManager.getText('cut'), desc: "Ctrl+X " + languageManager.getText('cut')},
            {value: 4, name: languageManager.getText('undo'), desc: "Ctrl+Z " + languageManager.getText('undo')},
            {value: 5, name: languageManager.getText('redo'), desc: "Ctrl+Y " + languageManager.getText('redo')},
            {value: 6, name: languageManager.getText('selectAll'), desc: "Ctrl+A " + languageManager.getText('selectAll')},
            {value: 7, name: languageManager.getText('delete'), desc: "Delete " + languageManager.getText('delete')},
            {value: 8, name: languageManager.getText('backspace'), desc: "Backspace " + languageManager.getText('backspace')},
            {value: 9, name: languageManager.getText('enter'), desc: "Enter " + languageManager.getText('enter')},
            {value: 10, name: languageManager.getText('escape'), desc: languageManager.getText('escape')},
            {value: 11, name: languageManager.getText('tab'), desc: languageManager.getText('tab')},
            {value: 12, name: languageManager.getText('pageUp'), desc: "Page Up"},
            {value: 13, name: languageManager.getText('pageDown'), desc: "Page Down"},
            {value: 14, name: languageManager.getText('mouseLeftPress'), desc: languageManager.getText('mouseLeftPress')},
            {value: 15, name: languageManager.getText('mouseLeftRelease'), desc: languageManager.getText('mouseLeftRelease')},
            {value: 16, name: languageManager.getText('mouseRightPress'), desc: languageManager.getText('mouseRightPress')},
            {value: 17, name: languageManager.getText('mouseRightRelease'), desc: languageManager.getText('mouseRightRelease')},
            {value: 19, name: languageManager.getText('mouseMoveLeft'), desc: languageManager.getText('mouseMoveLeft')},
            {value: 20, name: languageManager.getText('mouseMoveRight'), desc: languageManager.getText('mouseMoveRight')},
            {value: 21, name: languageManager.getText('mouseMoveUp'), desc: languageManager.getText('mouseMoveUp')},
            {value: 22, name: languageManager.getText('mouseMoveDown'), desc: languageManager.getText('mouseMoveDown')},
            {value: 24, name: languageManager.getText('mouseScrollUp'), desc: languageManager.getText('mouseScrollUp')},
            {value: 25, name: languageManager.getText('mouseScrollDown'), desc: languageManager.getText('mouseScrollDown')},
            {value: 27, name: languageManager.getText('mouseScrollLeft'), desc: languageManager.getText('mouseScrollLeft')},
            {value: 28, name: languageManager.getText('mouseScrollRight'), desc: languageManager.getText('mouseScrollRight')},
            {value: 29, name: languageManager.getText('volumeUp'), desc: languageManager.getText('volumeUp')},
            {value: 30, name: languageManager.getText('volumeDown'), desc: languageManager.getText('volumeDown')},
            {value: 31, name: languageManager.getText('muteToggle'), desc: languageManager.getText('muteToggle')},
            {value: 32, name: languageManager.getText('playPause'), desc: languageManager.getText('playPause')},
            {value: 33, name: languageManager.getText('nextTrack'), desc: languageManager.getText('nextTrack')},
            {value: 34, name: languageManager.getText('prevTrack'), desc: languageManager.getText('prevTrack')},
            {value: 35, name: languageManager.getText('stopPlay'), desc: languageManager.getText('stopPlay')}
        ];

        this.JoystickHIDFunctionOptions = [
            {value: 0, name: languageManager.getText('noFunction'), desc: languageManager.getText('noFunction')},
            {value: 18, name: languageManager.getText('mouseMove'), desc: languageManager.getText('mouseMove')},
            {value: 36, name: languageManager.getText('mapWASD'), desc: languageManager.getText('mapWASD')},
            {value: 37, name: languageManager.getText('arrowKeys'), desc: languageManager.getText('mapArrows')},
            {value: 23, name: languageManager.getText('mouseScrollVertical'), desc: languageManager.getText('mouseScrollVertical')},
            {value: 26, name: languageManager.getText('mouseScrollHorizontal'), desc: languageManager.getText('mouseScrollHorizontal')},
        ];

        this.EncoderHIDFunctionOptions = [
            {value: 0, name: languageManager.getText('noFunction'), desc: languageManager.getText('noFunction')},
            {value: 1, name: languageManager.getText('copy'), desc: "Ctrl+C " + languageManager.getText('copy')},
            {value: 2, name: languageManager.getText('paste'), desc: "Ctrl+V " + languageManager.getText('paste')},
            {value: 3, name: languageManager.getText('cut'), desc: "Ctrl+X " + languageManager.getText('cut')},
            {value: 4, name: languageManager.getText('undo'), desc: "Ctrl+Z " + languageManager.getText('undo')},
            {value: 5, name: languageManager.getText('redo'), desc: "Ctrl+Y " + languageManager.getText('redo')},
            {value: 6, name: languageManager.getText('selectAll'), desc: "Ctrl+A " + languageManager.getText('selectAll')},
            {value: 7, name: languageManager.getText('delete'), desc: "Delete " + languageManager.getText('delete')},
            {value: 8, name: languageManager.getText('backspace'), desc: "Backspace " + languageManager.getText('backspace')},
            {value: 9, name: languageManager.getText('enter'), desc: "Enter " + languageManager.getText('enter')},
            {value: 10, name: languageManager.getText('escape'), desc: languageManager.getText('escape')},
            {value: 11, name: languageManager.getText('tab'), desc: languageManager.getText('tab')},
            {value: 12, name: languageManager.getText('pageUp'), desc: "Page Up"},
            {value: 13, name: languageManager.getText('pageDown'), desc: "Page Down"},
            {value: 14, name: languageManager.getText('mouseLeftPress'), desc: languageManager.getText('mouseLeftPress')},
            {value: 15, name: languageManager.getText('mouseLeftRelease'), desc: languageManager.getText('mouseLeftRelease')},
            {value: 16, name: languageManager.getText('mouseRightPress'), desc: languageManager.getText('mouseRightPress')},
            {value: 17, name: languageManager.getText('mouseRightRelease'), desc: languageManager.getText('mouseRightRelease')},
            {value: 19, name: languageManager.getText('mouseMoveLeft'), desc: languageManager.getText('mouseMoveLeft')},
            {value: 20, name: languageManager.getText('mouseMoveRight'), desc: languageManager.getText('mouseMoveRight')},
            {value: 21, name: languageManager.getText('mouseMoveUp'), desc: languageManager.getText('mouseMoveUp')},
            {value: 22, name: languageManager.getText('mouseMoveDown'), desc: languageManager.getText('mouseMoveDown')},
            {value: 23, name: languageManager.getText('mouseScrollVertical'), desc: languageManager.getText('mouseScrollVertical')},
            {value: 24, name: languageManager.getText('mouseScrollUp'), desc: languageManager.getText('mouseScrollUp')},
            {value: 25, name: languageManager.getText('mouseScrollDown'), desc: languageManager.getText('mouseScrollDown')},
            {value: 26, name: languageManager.getText('mouseScrollHorizontal'), desc: languageManager.getText('mouseScrollHorizontal')},
            {value: 27, name: languageManager.getText('mouseScrollLeft'), desc: languageManager.getText('mouseScrollLeft')},
            {value: 28, name: languageManager.getText('mouseScrollRight'), desc: languageManager.getText('mouseScrollRight')},
            {value: 29, name: languageManager.getText('volumeUp'), desc: languageManager.getText('volumeUp')},
            {value: 30, name: languageManager.getText('volumeDown'), desc: languageManager.getText('volumeDown')},
            {value: 31, name: languageManager.getText('muteToggle'), desc: languageManager.getText('muteToggle')},
            {value: 32, name: languageManager.getText('playPause'), desc: languageManager.getText('playPause')},
            {value: 33, name: languageManager.getText('nextTrack'), desc: languageManager.getText('nextTrack')},
            {value: 34, name: languageManager.getText('prevTrack'), desc: languageManager.getText('prevTrack')},
            {value: 35, name: languageManager.getText('stopPlay'), desc: languageManager.getText('stopPlay')}
        ];

        this.AngleHIDFunctionOptions = [
            {value: 0, name: languageManager.getText('noFunction'), desc: languageManager.getText('noFunction')},
            {value: 23, name: languageManager.getText('mouseScrollVertical'), desc: languageManager.getText('mouseScrollVertical')},
            {value: 26, name: languageManager.getText('mouseScrollHorizontal'), desc: languageManager.getText('mouseScrollHorizontal')},
        ];
    }

    // 修复updateChainBusDisplay方法，添加更多调试信息
    updateChainBusDisplay(chainBusData) {
        if (!chainBusData) {
            console.log('Chain Bus数据为空');
            return;
        }
        
        // console.log('更新Chain Bus显示:', chainBusData);
        
        // 更新左Bus
        if (chainBusData.left_bus) {
            // console.log('处理左Bus数据:', chainBusData.left_bus);
            this.updateBusChainDisplay('left', chainBusData.left_bus);
        }
        
        // 更新右Bus
        if (chainBusData.right_bus) {
            // console.log('处理右Bus数据:', chainBusData.right_bus);
            this.updateBusChainDisplay('right', chainBusData.right_bus);
        }
    }

    // updateBusChainDisplay - 处理Chain Bus设备的详细显示
    updateBusChainDisplay(busName, busData) {
        // console.log(`更新${busName} Bus Chain显示:`, busData);
        
        // 更新chainBusState中的数据
        const busStateKey = busName + 'Bus';
        this.chainBusState[busStateKey].connected = busData.connected || false;
        this.chainBusState[busStateKey].deviceCount = busData.device_count || 0;
        const now = new Date();
        this.chainBusState[busStateKey].refreshTime = now.toTimeString().split(' ')[0];
        
        // 更新设备数量显示
        const deviceCountElement = document.getElementById(`${busName}BusDeviceCount`);
        if (deviceCountElement) {
            deviceCountElement.textContent = busData.device_count || 0;
            // console.log(`${busName} Bus设备数量:`, busData.device_count);
        } else {
            console.error(`未找到${busName} Bus设备数量元素`);
        }
        
        // 更新刷新时间显示
        const refreshTimeElement = document.getElementById(`${busName}BusRefreshTime`);
        if (refreshTimeElement) {
            refreshTimeElement.textContent = this.chainBusState[busStateKey].refreshTime;
        }
        
        // 处理设备列表
        const deviceContainer = document.getElementById(`${busName}BusDevices`);
        if (deviceContainer) {
            if (busData.device_count === 0 || busData.connected === false) {
                deviceContainer.innerHTML = '';
                this.deviceCache[busName].clear();
            }

            // 处理设备离线检测
            if (busData.connected_device_ids && Array.isArray(busData.connected_device_ids)) {
                this.removeOfflineDevices(busName, busData.connected_device_ids);
            }
            
            if (busData.devices && Array.isArray(busData.devices)) {
                // console.log(`${busName} Bus设备列表:`, busData.devices);
                this.updateDeviceList(busName, busData.devices);
            } else {
                // console.log(`${busName} Bus本次没有设备更新数据`);
                // 不清空设备列表，保持现有显示
            }
            
            // 在设备信息更新后，基于连接状态更新可视化显示
            if (busData.connected_device_ids && Array.isArray(busData.connected_device_ids)) {
                this.updateBusVisualizationFromConnectedDevices(busName, busData);
            }
        } else {
            // console.error(`未找到${busName} Bus设备容器`);
        }

        // 在处理完设备列表后，进行清理和验证
        if (busData.devices && Array.isArray(busData.devices)) {
            this.updateDeviceList(busName, busData.devices);
            
            // 清理可能的重复设备（防御性措施）
            setTimeout(() => {
                this.cleanupDuplicateDevices(busName);
                this.validateDeviceCache(busName);
            }, 100);
        }
        
        if (deviceContainer.children.length != busData.device_count) {
            // console.log(`${busName} Bus设备数量不一致，容器中有${deviceContainer.children.length}个设备，后端报告${busData.device_count}个设备`);
            const lastEnumerateKey = `${busName}BusChainEnumerate`;
            const now = Date.now();
            const lastEnumerateTime = this[lastEnumerateKey] || 0;
            const timeSinceLastEnumerate = now - lastEnumerateTime;
            const enumerateInterval = 2000; // 2秒间隔

            if (timeSinceLastEnumerate > enumerateInterval) {
                // console.log(`${busName} Bus触发设备枚举，距离上次枚举${timeSinceLastEnumerate}ms`);
                this.enumerateBusDevices(busName);
                this[lastEnumerateKey] = now;
            }
        }
    }

    // 增量更新设备列表 - 只更新有变化的设备，保持其他设备不变
    updateDeviceList(busName, devices) {
        const container = document.getElementById(`${busName}BusDevices`);
        if (!container) {
            console.error(`${busName} Bus设备容器不存在`);
            return;
        }
    
        // console.log(`增量更新${busName} Bus设备列表，收到设备数量:`, devices.length);
    
        // 如果没有收到任何设备数据，不做任何改动（保持现有显示）
        if (!devices || devices.length === 0) {
            // console.log(`${busName} Bus本次没有设备更新，保持现有显示`);
            return;
        }
    
        // 处理收到的设备列表（增量更新）
        devices.forEach((device, index) => {
            const deviceId = device.id;
            console.log(`处理设备 ${index + 1}:`, device);
    
            // 使用设备ID作为唯一标识符
            const cachedDevice = this.deviceCache[busName].get(deviceId);
            
            // 检查容器中是否已存在该设备ID的元素
            let deviceElement = container.querySelector(`[data-device-id="${deviceId}"]`);
            
            if (!cachedDevice || this.isDeviceUpdated(cachedDevice, device)) {
                console.log(`设备${deviceId}需要更新`);
                
                // 添加前端更新时间戳
                device.frontend_update_time = Date.now();
    
                // 检查是否有新事件
                if (device.last_event && device.event_time &&
                    (!cachedDevice ||
                     device.last_event !== cachedDevice.last_event ||
                     device.event_time !== cachedDevice.event_time)) {
                    console.log(`设备${deviceId}有新事件: ${device.last_event}`);
                    device.event_trigger_time = Date.now();
                    this.scheduleEventCleanup(busName, deviceId);
                } else if (cachedDevice) {
                    // 如果没有新事件，但缓存中有正在显示的事件
                    if (this.eventTimers[busName].has(deviceId) && cachedDevice.last_event) {
                        console.log(`设备${deviceId}保持现有事件显示: ${cachedDevice.last_event}`);
                        device.last_event = cachedDevice.last_event;
                        device.event_trigger_time = cachedDevice.event_trigger_time;
                        device.event_time = cachedDevice.event_time;
                    }
                }
    
                // 更新或创建设备卡片
                this.updateDeviceCard(busName, deviceId, device);
                
                // 更新缓存 - 使用设备ID作为key
                this.deviceCache[busName].set(deviceId, { ...device });
            } else {
                console.log(`设备${deviceId}无需更新`);
                // 保持原有的前端时间戳
                device.frontend_update_time = cachedDevice.frontend_update_time;
            }
        });
    
        // 检查容器是否为空（首次加载或所有设备都离线）
        if (container.children.length === 0) {
            container.innerHTML = '<div class="no-devices">No devices</div>';
        } else {
            // 移除"暂无设备"提示（如果存在）
            const noDevicesElement = container.querySelector('.no-devices');
            if (noDevicesElement) {
                noDevicesElement.remove();
            }
            // 按设备ID排序设备列表
            this.sortDeviceContainer(container);
        }
    }

    // 刷新设备容器数据以更新翻译
    refreshDeviceContainers() {
        this.initHIDFunctionOptions();

        ['left', 'right'].forEach(busName => {
            const container = document.getElementById(`${busName}BusDevices`);
            if (!container) return;

            container.querySelectorAll('.device-item').forEach(element => {
                const deviceId = parseInt(element.getAttribute('data-device-id'), 10);
                const device = this.deviceCache[busName].get(deviceId);
                if (!device) return;

                const wasHIDExpanded = element.querySelector('.hid-config-content') &&
                    !element.querySelector('.hid-config-content').classList.contains('hidden');
                const refreshEnabled = this.deviceRefreshEnabled[busName].get(deviceId) !== false;
                const supportsRGB = this.deviceSupportsRGB(device.type);

                this.renderDeviceElementFull(element, device, busName, refreshEnabled, supportsRGB);
                this.bindRefreshToggleEvents(element);

                if (wasHIDExpanded) {
                    const newConfigContent = element.querySelector('.hid-config-content');
                    const toggleIcon = element.querySelector('.hid-config-toggle .toggle-icon');
                    if (newConfigContent) {
                        newConfigContent.classList.remove('hidden');
                        if (toggleIcon) toggleIcon.textContent = '▲';
                    }
                }
            });
        });
    }

    updateDeviceCard(busName, deviceId, device) {
        const container = document.getElementById(`${busName}BusDevices`);
        let deviceElement = container.querySelector(`[data-device-id="${deviceId}"]`);
        
        if (!deviceElement) {
            // 创建新的设备卡片
            console.log(`创建新设备卡片: ID=${deviceId}, Type=${device.type}`);
            deviceElement = this.createDeviceElement(device, deviceId, busName);
            container.appendChild(deviceElement);
            
            // 初始化刷新开关状态为启用
            this.deviceRefreshEnabled[busName].set(deviceId, true);
            
            // 添加动画效果
            deviceElement.style.opacity = '0';
            deviceElement.style.transform = 'translateY(10px)';
            setTimeout(() => {
                deviceElement.style.transition = 'all 0.3s ease';
                deviceElement.style.opacity = '1';
                deviceElement.style.transform = 'translateY(0)';
            }, 50);
        } else {
            // 检查设备刷新开关状态
            const refreshEnabled = this.deviceRefreshEnabled[busName].get(deviceId);
            if (refreshEnabled === false) {
                console.log(`设备${deviceId}的数据刷新已关闭，跳过更新`);
                return;
            }
            
            // 更新现有设备卡片
            console.log(`更新现有设备卡片: ID=${deviceId}, Type=${device.type}`);
            deviceElement.classList.add('updating');
            this.updateDeviceElement(deviceElement, device, busName);
            
            // 同时更新可视化元素
            this.updateVisualElementIfExists(busName, deviceId, device);
            
            // 移除更新动画
            setTimeout(() => {
                deviceElement.classList.remove('updating');
            }, 500);
        }
    }

    createDeviceElement(device, deviceId, busName) {
        const deviceDiv = document.createElement('div');
        deviceDiv.className = 'device-item';
        deviceDiv.setAttribute('data-device-id', deviceId);
        deviceDiv.setAttribute('data-device-type', device.type);
        deviceDiv.setAttribute('data-supports-rgb', this.deviceSupportsRGB(device.type));
        deviceDiv.setAttribute('data-bus', busName);
    
        console.log(`创建设备元素: ID=${deviceId}, Type=${device.type}`);
        this.updateDeviceElement(deviceDiv, device, busName);
        return deviceDiv;
    }

    // 按设备ID排序设备容器中的元素
    sortDeviceContainer(container) {
        // 获取所有设备元素（排除"暂无设备"提示）
        const deviceElements = Array.from(container.children).filter(child =>
            child.classList.contains('device-item')
        );

        // 如果设备数量少于2个，无需排序
        if (deviceElements.length < 2) {
            return;
        }

        // 检查当前顺序是否已经正确
        let isAlreadySorted = true;
        for (let i = 0; i < deviceElements.length - 1; i++) {
            const idA = parseInt(deviceElements[i].getAttribute('data-device-id')) || 0;
            const idB = parseInt(deviceElements[i + 1].getAttribute('data-device-id')) || 0;
            if (idA > idB) {
                isAlreadySorted = false;
                break;
            }
        }

        // 如果已经排序，则不需要重新排序
        if (isAlreadySorted) {
            return;
        }

        // 按设备ID进行排序（转换为数字进行比较）
        deviceElements.sort((a, b) => {
            const idA = parseInt(a.getAttribute('data-device-id')) || 0;
            const idB = parseInt(b.getAttribute('data-device-id')) || 0;
            return idA - idB;
        });

        // 重新排列DOM元素顺序
        deviceElements.forEach(element => {
            container.appendChild(element);
        });

        // console.log(`设备列表已按ID排序，共${deviceElements.length}个设备`);
    }

    // 修复updateDeviceElement方法，确保内容正确显示
    updateDeviceElement(element, device, requestedBusName = null) {
        const hidConfigContent = element.querySelector('.hid-config-content');
        const wasHIDExpanded = hidConfigContent && !hidConfigContent.classList.contains('hidden');
        const existingControlPanel = element.querySelector('.device-control-panel');

        const busContainer = element.closest('.bus-status');
        const busName = requestedBusName || element.dataset.bus ||
            (busContainer?.classList.contains('left-bus') ? 'left' : 'right');
        const deviceId = parseInt(element.getAttribute('data-device-id'), 10);
        const refreshEnabled = this.deviceRefreshEnabled[busName] &&
            this.deviceRefreshEnabled[busName].get(deviceId) !== false;

        const supportsRGB = this.deviceSupportsRGB(device.type);
        element.setAttribute('data-device-type', device.type);
        element.setAttribute('data-bus', busName);

        const header = element.querySelector('.device-header');
        if (!header) {
            this.renderDeviceElementFull(element, device, busName, refreshEnabled, supportsRGB);
        } else {
            const updateTimeEl = element.querySelector('.last-update');
            if (updateTimeEl) {
                updateTimeEl.textContent = `${languageManager.getText('updateTime')}: ${this.formatUpdateTime(device.frontend_update_time)}`;
            }
            const detailsEl = element.querySelector('.device-details');
            if (detailsEl) {
                detailsEl.innerHTML = `
                    <span class="last-update">${languageManager.getText('updateTime')}: ${this.formatUpdateTime(device.frontend_update_time)}</span>
                    ${this.formatEventDisplay(device)}
                    ${this.formatDeviceData(device)}
                `;
            }
            if (!element.querySelector('.device-hid-config') && this.deviceSupportsHID(device.type)) {
                const hidHtml = this.createHIDConfigControl(device, busName);
                const wrapper = document.createElement('div');
                wrapper.innerHTML = hidHtml;
                const hidNode = wrapper.firstElementChild;
                if (existingControlPanel) {
                    existingControlPanel.before(hidNode);
                } else {
                    element.appendChild(hidNode);
                }
                this.bindHIDConfigEvents(element, device);
            }
            if (!existingControlPanel && this.deviceSupportsControlPanel(device.type)) {
                const panelHtml = this.createDeviceControlPanel(device, busName);
                const wrapper = document.createElement('div');
                wrapper.innerHTML = panelHtml;
                element.appendChild(wrapper.firstElementChild);
                this.bindDeviceControlEvents(element, device, busName);
            }
        }

        if (wasHIDExpanded) {
            const newConfigContent = element.querySelector('.hid-config-content');
            const toggleIcon = element.querySelector('.hid-config-toggle .toggle-icon');
            if (newConfigContent) {
                newConfigContent.classList.remove('hidden');
                if (toggleIcon) toggleIcon.textContent = '▲';
            }
        }

        this.bindRefreshToggleEvents(element);
    }

    renderDeviceElementFull(element, device, busName, refreshEnabled, supportsRGB) {
        const deviceDataHtml = this.formatDeviceData(device);
        const rgbControlHtml = supportsRGB ? this.createRGBControl(device) : '';
        const hidConfigHtml = this.deviceSupportsHID(device.type) ? this.createHIDConfigControl(device, busName) : '';
        const controlPanelHtml = this.deviceSupportsControlPanel(device.type) ? this.createDeviceControlPanel(device, busName) : '';

        element.innerHTML = `
            <div class="device-header">
                <div class="device-info-group">
                    <span class="device-id">ID: ${device.id}</span>
                    <span class="device-type">${device.type}</span>
                    <span class="device-uid">UID: ${this.formatDeviceUID(device.uid)}</span>
                    ${device.rgb_setting ? `<span class="rgb-status setting">${languageManager.getText('rgbSetting')}</span>` : ''}
                </div>
                <div class="refresh-toggle">
                    <label class="toggle-switch">
                        <input type="checkbox" class="refresh-checkbox" ${refreshEnabled ? 'checked' : ''}>
                        <span class="toggle-slider"></span>
                    </label>
                    <span class="toggle-label">${refreshEnabled ? languageManager.getText('dataRefresh') : languageManager.getText('pauseRefresh')}</span>
                </div>
            </div>
            <div class="device-details">
                <span class="last-update">${languageManager.getText('updateTime')}: ${this.formatUpdateTime(device.frontend_update_time)}</span>
                ${this.formatEventDisplay(device)}
                ${deviceDataHtml}
            </div>
            ${rgbControlHtml}
            ${hidConfigHtml}
            ${controlPanelHtml}
        `;

        if (supportsRGB) {
            this.bindRGBControlEvents(element, device);
        }
        if (this.deviceSupportsHID(device.type)) {
            this.bindHIDConfigEvents(element, device);
        }
        if (this.deviceSupportsControlPanel(device.type)) {
            this.bindDeviceControlEvents(element, device, busName);
        }
    }

    // 更新可视化元素（如果存在）
    updateVisualElementIfExists(busName, deviceId, device) {
        const visualContainer = document.getElementById(`${busName}BusDevicesVisual`);
        if (!visualContainer) {
            return; // 可视化容器不存在，跳过
        }

        const visualElement = visualContainer.querySelector(`[data-device-id="${deviceId}"]`);
        if (visualElement) {
            console.log(`更新设备 ${deviceId} 的可视化元素`);
            this.updateDeviceVisualElement(visualElement, device);
        }
    }
    
    // 检查设备是否支持RGB
    deviceSupportsRGB(deviceType) {
        const rgbSupportedTypes = ['KEY', 'PIR', 'SWITCH', 'JOYSTICK', 'ENCODER', 'TOF', 'ANGLE', 'UART', 'CHAIN_BUS',
            'PEDAL', 'MIC', 'BUZZER', 'ENV', 'IMU', 'DLIGHT'];
        return rgbSupportedTypes.includes(deviceType);
    }

    // 检查设备是否支持HID配置
    deviceSupportsHID(deviceType) {
        const hidSupportedTypes = ['KEY', 'JOYSTICK', 'ENCODER', 'ANGLE', 'PEDAL', 'MIC', 'SWITCH', 'PIR', 'DLIGHT'];
        return hidSupportedTypes.includes(deviceType);
    }

    deviceSupportsControlPanel(deviceType) {
        const panelTypes = ['BUZZER', 'MONO', 'RGB', 'SERVOS', 'CHAIN_BUS'];
        return panelTypes.includes(deviceType);
    }
    
    // 格式化设备数据显示
    formatDeviceData(device) {
        if (!device.device_data) return '';
        
        let dataHtml = '<div class="device-data">';
        
        switch (device.type) {
            case 'KEY':
                dataHtml += `<span class="data-item">${languageManager.getText('buttonStatus')}: ${device.device_data.button_status ? languageManager.getText('pressed') : languageManager.getText('released')}</span>`;
                break;
                
            case 'PIR':
                dataHtml += `<span class="data-item">${languageManager.getText('detectStatus')}: ${device.device_data.detect_status ? languageManager.getText('someoneDetected') : languageManager.getText('noOneDetected')}</span>`;
                if (device.device_data.trigger_count > 0) {
                    dataHtml += `<span class="data-item">${languageManager.getText('triggerCount')}: ${device.device_data.trigger_count}</span>`;
                }
                break;
                
            case 'JOYSTICK':
                dataHtml += `<span class="data-item">${languageManager.getText('xAxis')}: ${device.device_data.x_value || 0}</span>`;
                dataHtml += `<span class="data-item">${languageManager.getText('yAxis')}: ${device.device_data.y_value || 0}</span>`;
                dataHtml += `<span class="data-item">${languageManager.getText('button')}: ${device.device_data.button_status ? languageManager.getText('pressed') : languageManager.getText('released')}</span>`;
                break;
                
            case 'ENCODER':
                dataHtml += `<span class="data-item">${languageManager.getText('encoderValue')}: ${device.device_data.encoder_value || 0}</span>`;
                dataHtml += `<span class="data-item">${languageManager.getText('buttonStatus')}: ${device.device_data.button_status ? languageManager.getText('pressed') : languageManager.getText('released')}</span>`;
                break;
                
            case 'TOF':
                dataHtml += `<span class="data-item">${languageManager.getText('distance')}: ${device.device_data.distance || 0}mm</span>`;
                break;
                
            case 'ANGLE':
                dataHtml += `<span class="data-item">${languageManager.getText('angleADC')}: ${device.device_data.angle_value || 0}</span>`;
                break;
                
            case 'SWITCH':
                dataHtml += `<span class="data-item">${languageManager.getText('switchStatus')}: ${device.device_data.switch_status ? languageManager.getText('switchOn') : languageManager.getText('switchOff')}</span>`;
                if (device.device_data.switch_count > 0) {
                    dataHtml += `<span class="data-item">${languageManager.getText('switchCount')}: ${device.device_data.switch_count}</span>`;
                }
                break;

            case 'PEDAL':
                dataHtml += `<span class="data-item">${languageManager.getText('buttonStatus')}: ${device.device_data.button_status ? languageManager.getText('pressed') : languageManager.getText('released')}</span>`;
                dataHtml += `<span class="data-item">${languageManager.getText('switchStatus')}: ${device.device_data.switch_status ? languageManager.getText('switchOn') : languageManager.getText('switchOff')}</span>`;
                break;

            case 'MIC':
                dataHtml += `<span class="data-item">ADC: ${device.device_data.adc_value || 0}</span>`;
                dataHtml += `<span class="data-item">Threshold: ${device.device_data.threshold || 0}</span>`;
                break;

            case 'ENV': {
                const spaOk = device.device_data.spa_ok !== false;
                const pressure = device.device_data.pressure || 0;
                const altitude = device.device_data.altitude || 0;
                const pressureStr = (!spaOk && pressure === 0) ? '--' : `${(pressure / 10).toFixed(1)} hPa`;
                const altitudeStr = (!spaOk && altitude === 0) ? '--' : `${(altitude / 100).toFixed(2)} m`;
                dataHtml += `<span class="data-item">${languageManager.getText('temperature')}: ${((device.device_data.temperature || 0) / 100).toFixed(1)}°C</span>`;
                dataHtml += `<span class="data-item">${languageManager.getText('humidity')}: ${device.device_data.humidity || 0}%</span>`;
                dataHtml += `<span class="data-item">${languageManager.getText('pressure')}: ${pressureStr}</span>`;
                dataHtml += `<span class="data-item">${languageManager.getText('altitude')}: ${altitudeStr}</span>`;
                break;
            }

            case 'IMU':
                dataHtml += `<span class="data-item">${languageManager.getText('accel')}: ${device.device_data.ax}, ${device.device_data.ay}, ${device.device_data.az}</span>`;
                dataHtml += `<span class="data-item">${languageManager.getText('gyro')}: ${device.device_data.gx}, ${device.device_data.gy}, ${device.device_data.gz}</span>`;
                dataHtml += `<span class="data-item">${languageManager.getText('temperature')}: ${((device.device_data.temperature || 0) / 100).toFixed(1)}°C</span>`;
                break;

            case 'DLIGHT':
                dataHtml += `<span class="data-item">${languageManager.getText('lux')}: ${device.device_data.lux || 0}</span>`;
                break;

            case 'SERVOS': {
                const angles = device.device_data.angles || [];
                dataHtml += `<span class="data-item">${languageManager.getText('servoAngle')}: ${angles.join(', ')}</span>`;
                dataHtml += `<span class="data-item">${languageManager.getText('dcVoltage')}: ${device.device_data.dc_voltage || 0} mV</span>`;
                dataHtml += `<span class="data-item">${languageManager.getText('groveVoltage')}: ${device.device_data.grove_voltage || 0} mV</span>`;
                break;
            }

            case 'CHAIN_BUS': {
                const addrs = device.device_data.i2c_addrs || [];
                dataHtml += `<span class="data-item">${languageManager.getText('i2cAddrs')}: ${addrs.length ? addrs.map(a => '0x' + a.toString(16)).join(', ') : '-'}</span>`;
                break;
            }
                
            default:
                dataHtml += `<span class="data-item">${languageManager.getText('noData')}</span>`;
        }
        
        dataHtml += '</div>';
        return dataHtml;
    }
    
    // 创建RGB控制UI
    createRGBControl(device) {
        const currentColor = this.rgbIntToHex(device.rgb_color || 0x000000);
        
        return `
            <div class="device-rgb-control">
                <div class="rgb-input-group">
                    <label>${languageManager.getText('rgbColor')}</label>
                    <input type="color" class="device-color-picker" value="${currentColor}" 
                           data-device-id="${device.id}" ${device.rgb_setting ? 'disabled' : ''}>
                    <button class="rgb-set-btn" data-device-id="${device.id}" 
                            ${device.rgb_setting ? 'disabled' : ''}>
                        ${device.rgb_setting ? languageManager.getText('setting') : languageManager.getText('set')}
                    </button>
                </div>
            </div>
        `;
    }

    // 创建HID配置UI
    createHIDConfigControl(device, busName) {
        if (!device.hid_config) {
            return '';
        }

        let hidConfigHtml = `
            <div class="device-hid-config">
                <div class="hid-config-header">
                    <h4>${languageManager.getText('hidFunctionConfig')}</h4>
                    <button class="hid-config-toggle" data-device-id="${device.id}">
                        <span class="toggle-icon">▼</span>
                    </button>
                </div>
                <div class="hid-config-content hidden">
        `;

        switch (device.type) {
            case 'KEY':
                hidConfigHtml += this.createKeyHIDConfig(device);
                break;
            case 'JOYSTICK':
                hidConfigHtml += this.createJoystickHIDConfig(device, busName);
                break;
            case 'ENCODER':
                hidConfigHtml += this.createEncoderHIDConfig(device);
                break;
            case 'ANGLE':
                hidConfigHtml += this.createAngleHIDConfig(device);
                break;
            case 'PEDAL':
                hidConfigHtml += this.createPedalHIDConfig(device);
                break;
            case 'MIC':
                hidConfigHtml += this.createMicHIDConfig(device);
                break;
            case 'SWITCH':
                hidConfigHtml += this.createSwitchHIDConfig(device);
                break;
            case 'DLIGHT':
                hidConfigHtml += this.createDlightHIDConfig(device);
                break;
            case 'PIR':
                hidConfigHtml += this.createPirHIDConfig(device);
                break;
        }

        hidConfigHtml += `
                    <div class="hid-config-actions">
                        <button class="hid-apply-btn" data-device-id="${device.id}">${languageManager.getText('applyConfig')}</button>
                        <button class="hid-reset-btn" data-device-id="${device.id}">${languageManager.getText('resetConfig')}</button>
                    </div>
                </div>
            </div>
        `;

        return hidConfigHtml;
    }

    // 创建KEY设备的HID配置
    createKeyHIDConfig(device) {
        const config = device.hid_config || {};
        return `
            <div class="hid-config-group">
                <label>${languageManager.getText('singleClickFunction')}:</label>
                <span class="current-config">[${this.getHIDFunctionName(config.single_click ?? 1)}]</span>
                <select class="hid-function-select" data-config-key="single_click">
                    ${this.generateKeyHIDFunctionOptions(config.single_click ?? 1)}
                </select>
            </div>
            <div class="hid-config-group">
                <label>${languageManager.getText('doubleClickFunction')}:</label>
                <span class="current-config">[${this.getHIDFunctionName(config.double_click ?? 2)}]</span>
                <select class="hid-function-select" data-config-key="double_click">
                    ${this.generateKeyHIDFunctionOptions(config.double_click ?? 2)}
                </select>
            </div>
            <div class="hid-config-group">
                <label>${languageManager.getText('longPressFunction')}:</label>
                <span class="current-config">[${this.getHIDFunctionName(config.long_press ?? 6)}]</span>
                <select class="hid-function-select" data-config-key="long_press">
                    ${this.generateKeyHIDFunctionOptions(config.long_press ?? 6)}
                </select>
            </div>
            <div class="hid-config-group">
                <label>${languageManager.getText('pressFunction')}:</label>
                <span class="current-config">[${this.getHIDFunctionName(config.press_down ?? 0)}]</span>
                <select class="hid-function-select" data-config-key="press_down">
                    ${this.generateKeyHIDFunctionOptions(config.press_down ?? 0)}
                </select>
            </div>
            <div class="hid-config-group">
                <label>${languageManager.getText('releaseFunction')}:</label>
                <span class="current-config">[${this.getHIDFunctionName(config.press_release ?? 0)}]</span>
                <select class="hid-function-select" data-config-key="press_release">
                    ${this.generateKeyHIDFunctionOptions(config.press_release ?? 0)}
                </select>
            </div>
        `;
    }

    createPedalHIDConfig(device) {
        const config = device.hid_config || {};
        return `
            <div class="hid-config-group">
                <label>${languageManager.getText('singleClickFunction')}:</label>
                <span class="current-config">[${this.getHIDFunctionName(config.single_click ?? 9)}]</span>
                <select class="hid-function-select" data-config-key="single_click">
                    ${this.generateKeyHIDFunctionOptions(config.single_click ?? 9)}
                </select>
            </div>
            <div class="hid-config-group">
                <label>${languageManager.getText('pressFunction')}:</label>
                <span class="current-config">[${this.getHIDFunctionName(config.press_down ?? 14)}]</span>
                <select class="hid-function-select" data-config-key="press_down">
                    ${this.generateKeyHIDFunctionOptions(config.press_down ?? 14)}
                </select>
            </div>
            <div class="hid-config-group">
                <label>${languageManager.getText('releaseFunction')}:</label>
                <span class="current-config">[${this.getHIDFunctionName(config.press_release ?? 15)}]</span>
                <select class="hid-function-select" data-config-key="press_release">
                    ${this.generateKeyHIDFunctionOptions(config.press_release ?? 15)}
                </select>
            </div>
        `;
    }

    createMicHIDConfig(device) {
        const config = device.hid_config || {};
        const threshold = config.threshold ?? device.device_data?.threshold ?? 2000;
        const interval = config.trigger_interval_ms ?? 500;
        return `
            <div class="hid-config-group">
                <label>${languageManager.getText('micThreshold')}:</label>
                <input type="number" class="mic-threshold-input" data-config-key="threshold" value="${threshold}" min="0" max="4095">
            </div>
            <div class="hid-config-group">
                <label>${languageManager.getText('triggerInterval')}:</label>
                <input type="number" class="mic-interval-input" data-config-key="trigger_interval_ms" value="${interval}" min="50" max="5000">
            </div>
            <div class="hid-config-group">
                <label>${languageManager.getText('aboveThreshold')}:</label>
                <select class="hid-function-select" data-config-key="high_threshold_func">
                    ${this.generateKeyHIDFunctionOptions(config.high_threshold_func ?? 31)}
                </select>
            </div>
            <div class="hid-config-group">
                <label>${languageManager.getText('belowThreshold')}:</label>
                <select class="hid-function-select" data-config-key="low_threshold_func">
                    ${this.generateKeyHIDFunctionOptions(config.low_threshold_func ?? 0)}
                </select>
            </div>
        `;
    }

    createSwitchHIDConfig(device) {
        const config = device.hid_config || {};
        return `
            <div class="hid-config-group">
                <label>${languageManager.getText('switchOnAction')}:</label>
                <select class="hid-function-select" data-config-key="open_func">
                    ${this.generateKeyHIDFunctionOptions(config.open_func ?? 31)}
                </select>
            </div>
            <div class="hid-config-group">
                <label>${languageManager.getText('switchOffAction')}:</label>
                <select class="hid-function-select" data-config-key="close_func">
                    ${this.generateKeyHIDFunctionOptions(config.close_func ?? 0)}
                </select>
            </div>
        `;
    }

    createDlightHIDConfig(device) {
        const config = device.hid_config || {};
        return `
            <div class="hid-config-group">
                <label>${languageManager.getText('highThreshold')} (${languageManager.getText('lux')}):</label>
                <input type="number" class="dlight-high-input" data-config-key="high_threshold" value="${config.high_threshold ?? 500}" min="0">
            </div>
            <div class="hid-config-group">
                <label>${languageManager.getText('lowThreshold')} (${languageManager.getText('lux')}):</label>
                <input type="number" class="dlight-low-input" data-config-key="low_threshold" value="${config.low_threshold ?? 100}" min="0">
            </div>
            <div class="hid-config-group">
                <label>${languageManager.getText('luxTooHigh')}:</label>
                <select class="hid-function-select" data-config-key="lux_high_func">
                    ${this.generateKeyHIDFunctionOptions(config.lux_high_func ?? 31)}
                </select>
            </div>
            <div class="hid-config-group">
                <label>${languageManager.getText('luxTooLow')}:</label>
                <select class="hid-function-select" data-config-key="lux_low_func">
                    ${this.generateKeyHIDFunctionOptions(config.lux_low_func ?? 31)}
                </select>
            </div>
        `;
    }

    createPirHIDConfig(device) {
        const config = device.hid_config || {};
        return `
            <div class="hid-config-group">
                <label>${languageManager.getText('personEnter')}:</label>
                <select class="hid-function-select" data-config-key="person_come_func">
                    ${this.generateKeyHIDFunctionOptions(config.person_come_func ?? 32)}
                </select>
            </div>
            <div class="hid-config-group">
                <label>${languageManager.getText('personLeave')}:</label>
                <select class="hid-function-select" data-config-key="person_leave_func">
                    ${this.generateKeyHIDFunctionOptions(config.person_leave_func ?? 0)}
                </select>
            </div>
        `;
    }

    // 创建JOYSTICK设备的HID配置
    createJoystickHIDConfig(device, busName) {
        const config = device.hid_config || {};
        
        // 根据总线设置默认值：左边总线默认开启翻转，右边总线默认关闭翻转
        const defaultReverse = busName === 'left';
        const xyMoveReverse = config.xy_move_reverse !== undefined ? config.xy_move_reverse : defaultReverse;
        
        return `
            <div class="hid-config-group">
                <label>${languageManager.getText('singleClickFunction')}:</label>
                <span class="current-config">[${this.getHIDFunctionName(config.single_click ?? 0)}]</span>
                <select class="hid-function-select" data-config-key="single_click">
                    ${this.generateKeyHIDFunctionOptions(config.single_click ?? 0)}
                </select>
            </div>
            <div class="hid-config-group">
                <label>${languageManager.getText('doubleClickFunction')}:</label>
                <span class="current-config">[${this.getHIDFunctionName(config.double_click ?? 0)}]</span>
                <select class="hid-function-select" data-config-key="double_click">
                    ${this.generateKeyHIDFunctionOptions(config.double_click ?? 0)}
                </select>
            </div>
            <div class="hid-config-group">
                <label>${languageManager.getText('longPressFunction')}:</label>
                <span class="current-config">[${this.getHIDFunctionName(config.long_press ?? 0)}]</span>
                <select class="hid-function-select" data-config-key="long_press">
                    ${this.generateKeyHIDFunctionOptions(config.long_press ?? 0)}
                </select>
            </div>
            <div class="hid-config-group">
                <label>${languageManager.getText('pressFunction')}:</label>
                <span class="current-config">[${this.getHIDFunctionName(config.press_down ?? 16)}]</span>
                <select class="hid-function-select" data-config-key="press_down">
                    ${this.generateKeyHIDFunctionOptions(config.press_down ?? 16)}
                </select>
            </div>
            <div class="hid-config-group">
                <label>${languageManager.getText('releaseFunction')}:</label>
                <span class="current-config">[${this.getHIDFunctionName(config.press_release ?? 17)}]</span>
                <select class="hid-function-select" data-config-key="press_release">
                    ${this.generateKeyHIDFunctionOptions(config.press_release ?? 17)}
                </select>
            </div>
            <div class="hid-config-group">
                <label>${languageManager.getText('xyMoveFunction')}:</label>
                <span class="current-config">[${this.getJoystickHIDFunctionName(config.xy_move_func)}]</span>
                <select class="hid-function-select" data-config-key="xy_move_func">
                    ${this.generateJoystickHIDFunctionOptions(config.xy_move_func ?? 18)}
                </select>
            </div>
            <div class="hid-config-group">
                <label>
                    <input type="checkbox" class="hid-checkbox" data-config-key="xy_move_reverse" 
                           ${xyMoveReverse ? 'checked' : ''}>
                    ${languageManager.getText('directionReverse')}
                </label>
                <span class="current-config">[${xyMoveReverse ? languageManager.getText('enabled') : languageManager.getText('disabled')}]</span>
            </div>
        `;
    }

    // 创建ENCODER设备的HID配置
    createEncoderHIDConfig(device) {
        const config = device.hid_config || {};
        return `
            <div class="hid-config-group">
                <label>${languageManager.getText('singleClickFunction')}:</label>
                <span class="current-config">[${this.getHIDFunctionName(config.single_click ?? 0)}]</span>
                <select class="hid-function-select" data-config-key="single_click">
                    ${this.generateKeyHIDFunctionOptions(config.single_click ?? 0)}
                </select>
            </div>
            <div class="hid-config-group">
                <label>${languageManager.getText('doubleClickFunction')}:</label>
                <span class="current-config">[${this.getHIDFunctionName(config.double_click ?? 0)}]</span>
                <select class="hid-function-select" data-config-key="double_click">
                    ${this.generateKeyHIDFunctionOptions(config.double_click ?? 0)}
                </select>
            </div>
            <div class="hid-config-group">
                <label>${languageManager.getText('longPressFunction')}:</label>
                <span class="current-config">[${this.getHIDFunctionName(config.long_press ?? 0)}]</span>
                <select class="hid-function-select" data-config-key="long_press">
                    ${this.generateKeyHIDFunctionOptions(config.long_press ?? 0)}
                </select>
            </div>
            <div class="hid-config-group">
                <label>${languageManager.getText('pressFunction')}:</label>
                <span class="current-config">[${this.getHIDFunctionName(config.press_down ?? 14)}]</span>
                <select class="hid-function-select" data-config-key="press_down">
                    ${this.generateKeyHIDFunctionOptions(config.press_down ?? 14)}
                </select>
            </div>
            <div class="hid-config-group">
                <label>${languageManager.getText('releaseFunction')}:</label>
                <span class="current-config">[${this.getHIDFunctionName(config.press_release ?? 15)}]</span>
                <select class="hid-function-select" data-config-key="press_release">
                    ${this.generateKeyHIDFunctionOptions(config.press_release ?? 15)}
                </select>
            </div>
            <div class="hid-config-group">
                <label>${languageManager.getText('rotateClockwiseFunction')}:</label>
                <span class="current-config">[${this.getHIDFunctionName(config.rotate_cw_func ?? 23)}]</span>
                <select class="hid-function-select" data-config-key="rotate_cw_func">
                    ${this.generateEncoderHIDFunctionOptions(config.rotate_cw_func ?? 23)}
                </select>
            </div>
            <div class="hid-config-group">
                <label>${languageManager.getText('rotateCounterClockwiseFunction')}:</label>
                <span class="current-config">[${this.getHIDFunctionName(config.rotate_ccw_func ?? 23)}]</span>
                <select class="hid-function-select" data-config-key="rotate_ccw_func">
                    ${this.generateEncoderHIDFunctionOptions(config.rotate_ccw_func ?? 23)}
                </select>
            </div>
        `;
    }

    // 创建ANGLE设备的HID配置
    createAngleHIDConfig(device) {
        const config = device.hid_config || {};
        return `
            <div class="hid-config-group">
                <label>${languageManager.getText('angleChangeFunction')}:</label>
                <span class="current-config">[${this.getHIDFunctionName(config.angle_func ?? 23)}]</span>
                <select class="hid-function-select" data-config-key="angle_func">
                    ${this.generateAngleHIDFunctionOptions(config.angle_func ?? 23)}
                </select>
            </div>
        `;
    }

    // 生成HID功能选项
    generateHIDFunctionOptions(selectedValue) {
        return this.hidFunctionOptions.map(option => 
            `<option value="${option.value}" ${option.value === selectedValue ? 'selected' : ''} title="${option.desc}">
                ${option.name}
            </option>`
        ).join('');
    }

    generateKeyHIDFunctionOptions(selectedValue) {
        return this.KeyHIDFunctionOptions.map(option => 
            `<option value="${option.value}" ${option.value === selectedValue ? 'selected' : ''} title="${option.desc}">
                ${option.name}
            </option>`
        ).join('');
    }

    generateJoystickHIDFunctionOptions(selectedValue) {
        return this.JoystickHIDFunctionOptions.map(option => 
            `<option value="${option.value}" ${option.value === selectedValue ? 'selected' : ''} title="${option.desc}">
                ${option.name}
            </option>`
        ).join('');
    }

    generateEncoderHIDFunctionOptions(selectedValue) {
        return this.EncoderHIDFunctionOptions.map(option => 
            `<option value="${option.value}" ${option.value === selectedValue ? 'selected' : ''} title="${option.desc}">
                ${option.name}
            </option>`
        ).join('');
    }

    generateAngleHIDFunctionOptions(selectedValue) {
        return this.AngleHIDFunctionOptions.map(option => 
            `<option value="${option.value}" ${option.value === selectedValue ? 'selected' : ''} title="${option.desc}">
                ${option.name}
            </option>`
        ).join('');
    }

    // 获取HID功能名称
    getHIDFunctionName(value) {
        const numericValue = Number(value);
        const sources = [
            this.hidFunctionOptions,
            this.JoystickHIDFunctionOptions,
            this.EncoderHIDFunctionOptions,
            this.AngleHIDFunctionOptions,
            this.KeyHIDFunctionOptions
        ];
        for (const options of sources) {
            const option = options.find(opt => opt.value === numericValue);
            if (option) {
                return option.name;
            }
        }
        return languageManager.getText('unknownFunction') || '未知功能';
    }

    getJoystickHIDFunctionName(value) {
        const numericValue = Number(value ?? 18);
        const option = this.JoystickHIDFunctionOptions.find(opt => opt.value === numericValue);
        return option ? option.name : this.getHIDFunctionName(numericValue);
    }
    
    // 创建设备交互控制面板
    createDeviceControlPanel(device, busName) {
        let panel = `<div class="device-control-panel" data-bus="${busName}" data-device-id="${device.id}">`;
        panel += `<h4>${languageManager.getText('interactiveControl')}</h4>`;

        switch (device.type) {
            case 'BUZZER':
                panel += this.createBuzzerControlPanel(device);
                break;
            case 'MONO':
                panel += this.createMatrixControlPanel(device, 'mono');
                break;
            case 'RGB':
                panel += this.createMatrixControlPanel(device, 'rgb');
                break;
            case 'SERVOS':
                panel += this.createServoControlPanel(device);
                break;
            case 'CHAIN_BUS':
                panel += `<button class="ctrl-btn i2c-scan-btn">${languageManager.getText('scanI2C')}</button>`;
                break;
        }
        panel += '</div>';
        return panel;
    }

    createBuzzerControlPanel(device) {
        const notes = [
            { n: 'C4', v: 13 }, { n: 'D4', v: 15 }, { n: 'E4', v: 17 }, { n: 'F4', v: 18 },
            { n: 'G4', v: 20 }, { n: 'A4', v: 22 }, { n: 'B4', v: 24 }, { n: 'C5', v: 25 },
            { n: 'Rest', v: 0 }
        ];
        const keys = notes.map(note =>
            `<button type="button" class="ctrl-btn note-btn" data-note="${note.v}">${note.n}</button>`
        ).join('');
        const presetKeys = ['presetMario', 'presetDoorbell', 'presetScale', 'presetAlarm'];
        const presets = presetKeys.map((key, i) =>
            `<option value="${i}">${languageManager.getText(key)}</option>`
        ).join('');
        return `
            <div class="buzzer-tabs">
                <button type="button" class="buzzer-tab active" data-tab="note">${languageManager.getText('buzzerSingleNote')}</button>
                <button type="button" class="buzzer-tab" data-tab="tone">${languageManager.getText('buzzerCustomTone')}</button>
                <button type="button" class="buzzer-tab" data-tab="seq">${languageManager.getText('buzzerMelody')}</button>
            </div>
            <div class="buzzer-tab-panel buzzer-tab-note">
                <div class="buzzer-keys">${keys}</div>
            </div>
            <div class="buzzer-tab-panel buzzer-tab-tone hidden">
                <div class="buzzer-tone-row">
                    <label>${languageManager.getText('frequency')}</label>
                    <input type="number" class="buzzer-freq" value="1000" min="100" max="10000">
                    <label>${languageManager.getText('dutyCycle')}</label>
                    <input type="number" class="buzzer-duty" value="50" min="1" max="100">
                    <label>${languageManager.getText('duration')}</label>
                    <input type="number" class="buzzer-duration" value="300" min="50" max="5000">
                    <button type="button" class="ctrl-btn buzzer-play-btn">${languageManager.getText('playTone')}</button>
                </div>
            </div>
            <div class="buzzer-tab-panel buzzer-tab-seq hidden">
                <div class="buzzer-seq-controls">
                    <select class="buzzer-seq-preset">${presets}</select>
                    <label><input type="checkbox" class="buzzer-seq-loop"> ${languageManager.getText('loop')}</label>
                    <button type="button" class="ctrl-btn buzzer-seq-play">${languageManager.getText('playEffect')}</button>
                </div>
            </div>
            <button type="button" class="ctrl-btn buzzer-global-stop">${languageManager.getText('stop')}</button>
        `;
    }

    createMatrixControlPanel(device, mode) {
        let grid = '';
        for (let y = 0; y < 8; y++) {
            for (let x = 0; x < 8; x++) {
                grid += `<div class="matrix-cell" data-x="${x}" data-y="${y}"></div>`;
            }
        }
        const colorPicker = mode === 'rgb'
            ? `<label>${languageManager.getText('scrollColor')}</label><input type="color" class="matrix-color" value="#ff0000">`
            : '';
        const scrollColorPicker = mode === 'rgb'
            ? `<label>${languageManager.getText('scrollColor')}</label><input type="color" class="matrix-scroll-color" value="#ff0000">`
            : '';
        const syncLabel = mode === 'mono'
            ? languageManager.getText('syncBusMono')
            : languageManager.getText('syncBusRgb');
        return `
            <div class="matrix-tabs">
                <button type="button" class="matrix-tab active" data-tab="draw">${languageManager.getText('pixelDraw')}</button>
                <button type="button" class="matrix-tab" data-tab="scroll">${languageManager.getText('scroll')}</button>
            </div>
            <div class="matrix-tab-panel matrix-tab-draw">
                <div class="matrix-grid ${mode}-grid" data-mode="${mode}">${grid}</div>
                ${colorPicker}
                <div class="matrix-actions">
                    <button type="button" class="ctrl-btn matrix-clear-btn">${languageManager.getText('clearScreen')}</button>
                </div>
                <p class="matrix-hint">${languageManager.getText('drawPixel')}</p>
            </div>
            <div class="matrix-tab-panel matrix-tab-scroll hidden">
                <input type="text" class="matrix-scroll-text" placeholder="${languageManager.getText('scrollTextPlaceholder')}" value="Hello World!  ">
                <label>${languageManager.getText('direction')}</label>
                <select class="matrix-scroll-dir">
                    <option value="0">${languageManager.getText('scrollDirLeft')}</option>
                    <option value="1">${languageManager.getText('scrollDirRight')}</option>
                    <option value="2">${languageManager.getText('scrollDirUp')}</option>
                    <option value="3">${languageManager.getText('scrollDirDown')}</option>
                </select>
                <label>${languageManager.getText('scrollMode')}</label>
                <select class="matrix-scroll-mode">
                    <option value="0">${languageManager.getText('scrollOnce')}</option>
                    <option value="1" selected>${languageManager.getText('scrollLoop')}</option>
                    <option value="2">${languageManager.getText('scrollPingPong')}</option>
                </select>
                <label>${languageManager.getText('intervalMs')}</label>
                <input type="number" class="matrix-scroll-interval" value="120" min="50" max="2000">
                ${scrollColorPicker}
                <label><input type="checkbox" class="matrix-scroll-sync"> ${syncLabel}</label>
                <button type="button" class="ctrl-btn matrix-scroll-btn">${languageManager.getText('scrollText')}</button>
            </div>
        `;
    }

    createServoControlPanel(device) {
        const angles = device.device_data?.angles || [90, 90, 90, 90, 90, 90, 90, 90];
        let sliders = '';
        for (let i = 0; i < 8; i++) {
            const angle = angles[i] ?? 90;
            sliders += `
                <div class="servo-row">
                    <label>${languageManager.getText('servoChannel')} ${i}</label>
                    <input type="range" class="servo-slider" data-gpio="${i}" min="0" max="180" value="${angle}">
                    <span class="servo-value">${angle}°</span>
                </div>`;
        }
        return `<div class="servo-panel">${sliders}</div>`;
    }

    bindDeviceControlEvents(element, device, busName) {
        const panel = element.querySelector('.device-control-panel');
        if (!panel || panel.dataset.bound === '1') return;
        panel.dataset.bound = '1';

        panel.querySelectorAll('.buzzer-tab').forEach(tab => {
            tab.addEventListener('click', () => {
                panel.querySelectorAll('.buzzer-tab').forEach(t => t.classList.remove('active'));
                panel.querySelectorAll('.buzzer-tab-panel').forEach(p => p.classList.add('hidden'));
                tab.classList.add('active');
                const target = panel.querySelector(`.buzzer-tab-${tab.dataset.tab}`);
                if (target) target.classList.remove('hidden');
            });
        });

        panel.querySelectorAll('.note-btn').forEach(btn => {
            btn.addEventListener('click', (e) => {
                e.preventDefault();
                const note = parseInt(btn.dataset.note, 10);
                this.sendDeviceCommand('buzzer_play', busName, device.id, { note, duration: 300 });
            });
        });

        const playBtn = panel.querySelector('.buzzer-play-btn');
        if (playBtn) {
            playBtn.addEventListener('click', (e) => {
                e.preventDefault();
                const freq = parseInt(panel.querySelector('.buzzer-freq')?.value || '1000', 10);
                const duty = parseInt(panel.querySelector('.buzzer-duty')?.value || '50', 10);
                const duration = parseInt(panel.querySelector('.buzzer-duration')?.value || '300', 10);
                this.sendDeviceCommand('buzzer_play', busName, device.id, { freq, duty, duration });
            });
        }

        panel.querySelector('.buzzer-seq-play')?.addEventListener('click', () => {
            const preset = parseInt(panel.querySelector('.buzzer-seq-preset')?.value || '0', 10);
            const loop = panel.querySelector('.buzzer-seq-loop')?.checked ?? false;
            this.sendDeviceCommand('buzzer_seq_play', busName, device.id, { preset, loop });
        });
        panel.querySelector('.buzzer-global-stop')?.addEventListener('click', () => {
            this.sendDeviceCommand('buzzer_seq_stop', busName, device.id, {});
            this.sendDeviceCommand('buzzer_stop', busName, device.id, {});
        });

        const grid = panel.querySelector('.matrix-grid');
        const matrixMode = grid?.dataset.mode;

        panel.querySelectorAll('.matrix-tab').forEach(tab => {
            tab.addEventListener('click', () => {
                panel.querySelectorAll('.matrix-tab').forEach(t => t.classList.remove('active'));
                panel.querySelectorAll('.matrix-tab-panel').forEach(p => p.classList.add('hidden'));
                tab.classList.add('active');
                const target = panel.querySelector(`.matrix-tab-${tab.dataset.tab}`);
                if (target) {
                    target.classList.remove('hidden');
                    if (tab.dataset.tab === 'draw' && matrixMode === 'mono') {
                        this.sendDeviceCommand('mono_clear', busName, device.id, {});
                    }
                }
            });
        });

        if (grid) {
            const mode = grid.dataset.mode;
            grid.querySelectorAll('.matrix-cell').forEach(cell => {
                cell.addEventListener('click', () => {
                    const x = parseInt(cell.dataset.x, 10);
                    const y = parseInt(cell.dataset.y, 10);
                    if (mode === 'mono') {
                        cell.classList.toggle('on');
                        this.sendDeviceCommand('mono_draw', busName, device.id, { x, y, state: cell.classList.contains('on') });
                    } else {
                        const colorHex = panel.querySelector('.matrix-color')?.value || '#ff0000';
                        const rgb565 = this.hexToRgb565(colorHex);
                        cell.style.backgroundColor = colorHex;
                        this.sendDeviceCommand('rgb_draw', busName, device.id, { x, y, color: rgb565 });
                    }
                });
            });
            panel.querySelector('.matrix-clear-btn')?.addEventListener('click', () => {
                grid.querySelectorAll('.matrix-cell').forEach(c => {
                    c.classList.remove('on');
                    c.style.backgroundColor = '';
                });
                this.sendDeviceCommand(mode === 'mono' ? 'mono_clear' : 'rgb_clear', busName, device.id, {});
            });
            panel.querySelector('.matrix-scroll-btn')?.addEventListener('click', () => {
                const text = panel.querySelector('.matrix-scroll-text')?.value || 'M5Stack';
                const dir = parseInt(panel.querySelector('.matrix-scroll-dir')?.value || '0', 10);
                const scrollMode = parseInt(panel.querySelector('.matrix-scroll-mode')?.value || '1', 10);
                const interval = parseInt(panel.querySelector('.matrix-scroll-interval')?.value || '120', 10);
                const syncAll = panel.querySelector('.matrix-scroll-sync')?.checked ?? false;
                const payload = { text, dir, mode: scrollMode, interval };
                if (mode === 'rgb') {
                    const colorEl = panel.querySelector('.matrix-scroll-color') || panel.querySelector('.matrix-color');
                    payload.color = this.hexToRgb565(colorEl?.value || '#ff0000');
                }
                let cmd = mode === 'mono' ? 'mono_scroll' : 'rgb_scroll';
                if (syncAll) {
                    cmd = mode === 'mono' ? 'mono_scroll_all' : 'rgb_scroll_all';
                }
                this.sendDeviceCommand(cmd, busName, device.id, payload);
            });
        }

        panel.querySelectorAll('.servo-slider').forEach(slider => {
            slider.addEventListener('input', () => {
                const valSpan = slider.parentElement.querySelector('.servo-value');
                if (valSpan) valSpan.textContent = `${slider.value}°`;
            });
            slider.addEventListener('change', () => {
                this.sendDeviceCommand('servo_set_angle', busName, device.id, {
                    gpio: parseInt(slider.dataset.gpio, 10),
                    angle: parseInt(slider.value, 10)
                });
            });
        });

        panel.querySelector('.i2c-scan-btn')?.addEventListener('click', () => {
            this.sendDeviceCommand('unitbus_i2c_scan', busName, device.id, {});
        });
    }

    sendDeviceCommand(type, busName, deviceId, params) {
        this.sendMessage({ type, bus: busName, device_id: deviceId, ...params });
    }

    hexToRgb565(hex) {
        const r = parseInt(hex.slice(1, 3), 16);
        const g = parseInt(hex.slice(3, 5), 16);
        const b = parseInt(hex.slice(5, 7), 16);
        return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
    }

    // 绑定RGB控制事件
    bindRGBControlEvents(element, device) {
        const colorPicker = element.querySelector('.device-color-picker');
        const setButton = element.querySelector('.rgb-set-btn');
        
        if (colorPicker && setButton) {
            setButton.addEventListener('click', () => {
                if (!device.rgb_setting) {
                    const color = this.hexToRgbInt(colorPicker.value);
                    this.setChainDeviceRGB(element, device.id, color);
                }
            });
        }
    }

    // 绑定HID配置事件
    bindHIDConfigEvents(element, device) {
        const toggleButton = element.querySelector('.hid-config-toggle');
        const configContent = element.querySelector('.hid-config-content');
        const applyButton = element.querySelector('.hid-apply-btn');
        
        // 绑定展开/收起事件
        if (toggleButton && configContent) {
            toggleButton.addEventListener('click', () => {
                const isVisible = !configContent.classList.contains('hidden');
                if (isVisible) {
                    configContent.classList.add('hidden');
                } else {
                    configContent.classList.remove('hidden');
                }
                const toggleIcon = toggleButton.querySelector('.toggle-icon');
                if (toggleIcon) {
                    toggleIcon.textContent = isVisible ? '▼' : '▲';
                }
            });
        }

        // 绑定应用配置按钮事件
        if (applyButton) {
            applyButton.addEventListener('click', () => {
                this.applyHIDConfig(element, device);
            });
        }

        // 绑定恢复默认配置按钮事件
        const resetButton = element.querySelector('.hid-reset-btn');
        if (resetButton) {
            resetButton.addEventListener('click', () => {
                this.resetHIDConfig(element, device);
            });
        }

        // 绑定复选框事件（用于启用/禁用相关控件）
        const checkboxes = element.querySelectorAll('.hid-checkbox');
        checkboxes.forEach(checkbox => {
            checkbox.addEventListener('change', () => {
                this.handleHIDCheckboxChange(element, checkbox);
            });
        });
    }

    // 处理HID配置复选框变化
    handleHIDCheckboxChange(element, checkbox) {
        const configKey = checkbox.getAttribute('data-config-key');
        
        if (configKey === 'xy_axis_enabled') {
            // 启用/禁用XY轴功能选择
            const xyMoveSelect = element.querySelector('select[data-config-key="xy_move_func"]');
            if (xyMoveSelect) {
                xyMoveSelect.disabled = !checkbox.checked;
            }
        } else if (configKey === 'enabled') {
            // 启用/禁用角度功能选择
            const angleFuncSelect = element.querySelector('select[data-config-key="angle_func"]');
            if (angleFuncSelect) {
                angleFuncSelect.disabled = !checkbox.checked;
            }
        }
    }

    // 应用HID配置
    applyHIDConfig(element, device) {
        const busContainer = element.closest('.bus-status');
        const isLeftBus = busContainer && busContainer.classList.contains('left-bus');
        const busName = isLeftBus ? 'left' : 'right';
        
        // 收集配置数据
        const config = {};
        
        // 收集下拉框配置
        const selects = element.querySelectorAll('.hid-function-select');
        selects.forEach(select => {
            const key = select.getAttribute('data-config-key');
            config[key] = parseInt(select.value);
        });
        
        // 收集复选框配置
        const checkboxes = element.querySelectorAll('.hid-checkbox');
        checkboxes.forEach(checkbox => {
            const key = checkbox.getAttribute('data-config-key');
            config[key] = checkbox.checked;
        });

        element.querySelectorAll('input[data-config-key]').forEach(input => {
            const key = input.getAttribute('data-config-key');
            config[key] = parseInt(input.value, 10);
        });
        
        // 发送配置到后端
        this.sendHIDConfig(busName, device.id, config);

        if (device.type === 'MIC') {
            this.sendMessage({
                type: 'set_mic_config',
                bus: busName,
                device_id: device.id,
                threshold: config.threshold,
                trigger_interval_ms: config.trigger_interval_ms
            });
        }
        if (device.type === 'DLIGHT') {
            this.sendMessage({
                type: 'set_dlight_config',
                bus: busName,
                device_id: device.id,
                high_threshold: config.high_threshold,
                low_threshold: config.low_threshold
            });
        }
    }

    // 发送HID配置到后端
    sendHIDConfig(busName, deviceId, config) {
        if (!this.websocket || this.websocket.readyState !== WebSocket.OPEN) {
            console.error('WebSocket未连接，无法发送HID配置');
            return;
        }

        const message = {
            type: 'set_device_hid_config',
            bus: busName,
            device_id: deviceId,
            config: config
        };

        // console.log('发送HID配置消息:', message);

        // 发送配置到后端，后端会通过状态更新消息返回最新配置
        this.websocket.send(JSON.stringify(message));
    }

    // 恢复HID配置为默认值
    resetHIDConfig(element, device) {
        const busContainer = element.closest('.bus-status');
        const isLeftBus = busContainer && busContainer.classList.contains('left-bus');
        const busName = isLeftBus ? 'left' : 'right';
        
        // 确认对话框
        if (!confirm(languageManager.getText('confirmResetHID').replace('{deviceId}', device.id))) {
            return;
        }
        
        console.log(`恢复设备 ${device.id} 的HID配置为默认值`);
        
        // 发送恢复默认配置命令到后端
        this.sendResetHIDConfig(busName, device.id);
        
        // 暂时禁用按钮
        const resetButton = element.querySelector('.hid-reset-btn');
        if (resetButton) {
            resetButton.disabled = true;
            resetButton.textContent = languageManager.getText('restoring');
            setTimeout(() => {
                resetButton.disabled = false;
                resetButton.textContent = languageManager.getText('restoreDefault');
            }, 3000);
        }
    }

    // 发送恢复默认HID配置命令到后端
    sendResetHIDConfig(busName, deviceId) {
        if (!this.websocket || this.websocket.readyState !== WebSocket.OPEN) {
            console.error('WebSocket未连接，无法发送恢复默认配置命令');
            return;
        }

        const message = {
            type: 'reset_device_hid_config',
            bus: busName,
            device_id: deviceId
        };

        console.log('发送恢复默认HID配置消息:', message);

        // 发送命令到后端
        this.websocket.send(JSON.stringify(message));
    }



    bindChainBusRGBControlEvents(element) {
        const colorPicker = element.querySelector('.device-color-picker');
        const setButton = element.querySelector('.rgb-set-btn');
        
        if (colorPicker && setButton) {
            setButton.addEventListener('click', () => {
                const color = this.hexToRgbInt(colorPicker.value);
                this.setChainBusRGB(element, color);
            });
        }
    }
    
    // 绑定刷新开关事件
    bindRefreshToggleEvents(element) {
        const refreshCheckbox = element.querySelector('.refresh-checkbox');
        if (refreshCheckbox) {
            refreshCheckbox.addEventListener('change', () => {
                // 确定设备所在的总线和设备ID
                const busContainer = element.closest('.bus-status');
                const isLeftBus = busContainer.classList.contains('left-bus');
                const busName = isLeftBus ? 'left' : 'right';
                const deviceId = parseInt(element.getAttribute('data-device-id'));
    
                // 更新刷新状态
                const refreshEnabled = refreshCheckbox.checked;
                this.deviceRefreshEnabled[busName].set(deviceId, refreshEnabled);
    
                console.log(`设备${deviceId}的数据刷新状态已设置为: ${refreshEnabled ? '启用' : '禁用'}`);
    
                // 更新开关标签文本
                const toggleLabel = element.querySelector('.toggle-label');
                if (toggleLabel) {
                    toggleLabel.textContent = refreshEnabled ? languageManager.getText('dataRefresh') : languageManager.getText('pauseRefresh');
                }
            });
        }
    }
    
    // 设置Chain设备RGB颜色
    setChainDeviceRGB(element, deviceId, color) {
        // 确定设备所在的总线
        const busContainer = element.closest('.bus-status');
        const isLeftBus = busContainer.classList.contains('left-bus');
        const busName = isLeftBus ? 'left' : 'right';
    
        console.log(`设置${busName} Bus设备 ${deviceId} RGB颜色: 0x${color.toString(16).padStart(6, '0').toUpperCase()}`);
    
        // 发送RGB设置命令
        this.sendMessage({
            type: 'set_chain_rgb',
            bus: busName,
            device_id: deviceId,
            color: color
        });
    
        // 暂时禁用控件
        const colorPicker = element.querySelector('.device-color-picker');
        const setButton = element.querySelector('.rgb-set-btn');
        if (colorPicker && setButton) {
            colorPicker.disabled = true;
            setButton.disabled = true;
            setButton.textContent = languageManager.getText('setting');
        }
    
        // 5秒后自动恢复（防止设备无响应导致永久禁用）
        setTimeout(() => {
            if (colorPicker && setButton) {
                colorPicker.disabled = false;
                setButton.disabled = false;
                setButton.textContent = languageManager.getText('set');
            }
        }, 5000);
    }

    setChainBusRGB(busName, color) {
        this.sendMessage({
            type: 'set_chain_bus_rgb',
            bus: busName,
            color: color
        });

        // 暂时禁用控件
        const colorPicker = document.getElementById(`${busName}BusColorPicker`);
        const setButton = document.getElementById(`${busName}BusRgbBtn`);
        
        if (colorPicker && setButton) {
            colorPicker.disabled = true;
            setButton.disabled = true;
            setButton.textContent = languageManager.getText('setting');
        }

        // 5秒后自动恢复（防止设备无响应导致永久禁用）
        setTimeout(() => {
            if (colorPicker && setButton) {
                colorPicker.disabled = false;
                setButton.disabled = false;
                setButton.textContent = languageManager.getText('set');
            }
        }, 5000);
    }

    // 调度事件清理（5秒后自动移除事件显示）
    scheduleEventCleanup(busName, deviceId) {
        // 清除已存在的定时器
        const existingTimer = this.eventTimers[busName].get(deviceId);
        if (existingTimer) {
            clearTimeout(existingTimer);
        }
    
        // 设置新的5秒定时器
        const timerId = setTimeout(() => {
            this.clearDeviceEvent(busName, deviceId);
        }, 5000);
        
        this.eventTimers[busName].set(deviceId, timerId);
    }
    
    // 清理设备事件显示
    clearDeviceEvent(busName, deviceId) {
        const cachedDevice = this.deviceCache[busName].get(deviceId);
        if (cachedDevice && cachedDevice.last_event) {
            // 清除事件相关字段
            delete cachedDevice.last_event;
            delete cachedDevice.event_trigger_time;
    
            // 更新设备卡片显示
            this.updateDeviceCard(busName, deviceId, cachedDevice);
            
            // 更新缓存
            this.deviceCache[busName].set(deviceId, { ...cachedDevice });
        }
    
        // 清除定时器
        this.eventTimers[busName].delete(deviceId);
    }
        
    
    // 移除离线设备
    removeOfflineDevices(busName, connectedDeviceIds) {
        const connectedIdSet = new Set(connectedDeviceIds.map(id => Number(id)));
        console.log(`${busName} Bus当前连接的设备ID:`, connectedDeviceIds);
    
        // 检查缓存中的设备，移除不在连接列表中的设备
        const devicesToRemove = [];
        this.deviceCache[busName].forEach((device, deviceId) => {
            if (!connectedIdSet.has(Number(deviceId))) {
                console.log(`设备 ${deviceId} 已离线，准备移除`);
                devicesToRemove.push(deviceId);
            }
        });
    
        // 移除离线设备
        devicesToRemove.forEach(deviceId => {
            // 清理事件定时器
            const existingTimer = this.eventTimers[busName].get(deviceId);
            if (existingTimer) {
                clearTimeout(existingTimer);
                this.eventTimers[busName].delete(deviceId);
            }
    
            // 清理刷新状态
            this.deviceRefreshEnabled[busName].delete(deviceId);
            
            // 从DOM中移除设备卡片
            this.removeDeviceCard(busName, deviceId);
            
            // 从缓存中移除
            this.deviceCache[busName].delete(deviceId);
        });
    
        if (devicesToRemove.length > 0) {
            console.log(`${busName} Bus移除了 ${devicesToRemove.length} 个离线设备`);
        }
    }
    
    // 检查设备是否更新
    isDeviceUpdated(cachedDevice, newDevice) {
        // 比较关键字段，忽略时间戳相关字段
        if (cachedDevice.id !== newDevice.id) return true;
        if (cachedDevice.type !== newDevice.type) return true;
        if (cachedDevice.connected !== newDevice.connected) return true;
        // last_update_time 已移除，不再比较后端时间戳
        if (cachedDevice.rgb_color !== newDevice.rgb_color) return true;
        if (cachedDevice.rgb_setting !== newDevice.rgb_setting) return true;
        if (cachedDevice.communication_flag !== newDevice.communication_flag) return true;

        // 比较HID配置
        if (newDevice.hid_config && cachedDevice.hid_config) {
            const newHIDConfig = JSON.stringify(newDevice.hid_config);
            const cachedHIDConfig = JSON.stringify(cachedDevice.hid_config);
            if (newHIDConfig !== cachedHIDConfig) return true;
        } else if (newDevice.hid_config !== cachedDevice.hid_config) {
            return true;
        }
        
        // 比较设备数据（如果存在）
        if (newDevice.device_data && cachedDevice.device_data) {
            const newData = JSON.stringify(newDevice.device_data);
            const cachedData = JSON.stringify(cachedDevice.device_data);
            if (newData !== cachedData) return true;
        } else if (newDevice.device_data !== cachedDevice.device_data) {
            return true;
        }
        
        // 检查是否有新事件（比较事件内容和后端事件时间）
        // 注意：不比较前端维护的 event_trigger_time 字段
        if (newDevice.last_event && newDevice.event_time) {
            // 如果没有缓存的事件，或者事件内容不同，或者后端事件时间不同（重复触发相同事件）
            if (!cachedDevice.last_event || 
                newDevice.last_event !== cachedDevice.last_event ||
                newDevice.event_time !== cachedDevice.event_time) {
                return true;
            }
        }
        
        return false;
    }
    
    // 移除设备卡片
    removeDeviceCard(busName, deviceId) {
        const container = document.getElementById(`${busName}BusDevices`);
        const deviceElement = container.querySelector(`[data-device-id="${deviceId}"]`);
        if (deviceElement) {
            console.log(`移除设备卡片: ID=${deviceId}`);
            deviceElement.remove();
        }
    }
    
    initStatusElements() {
        // 初始化标题栏中的状态显示元素
        this.wsStatusElement = document.getElementById('ws-status');
        this.lastUpdateElement = document.getElementById('lastUpdate');
        this.statusDotElement = document.querySelector('.status-dot');
        this.statusTextElement = document.querySelector('.status-text');
    }

    init() {
        // 初始化状态
        this.dualkeyState = {
            leftKey: false,
            rightKey: false,
            leftKeyColor: 0x000000,  // 红色
            rightKeyColor: 0x000000, // 绿色
            usbVoltage: 0.0,
            usbConnected: true,
            usbMode: 0, // 0:HID, 1:BLE, 2:CDC
            batteryVoltage: 3.7,
            batteryPercentage: 75,
            charge_status: 0, // 0:未充电, 1:充电中, 2:充满
            dipSwitch: 0, // 0:center, 1:left, 2:right
            switch1Value: 0,
            switch2Value: 0,
            // 蓝牙状态
            bluetoothConnected: false,
            bluetoothDeviceName: "Chain DualKey",
            bluetoothPairingStatus: 0, // 0:未配对, 1:配对中, 2:已配对
            bluetoothAdvStatus: false, // 蓝牙广播状态: false=未广播, true=正在广播
            // HID按键映射
            currentKeyMapping: 9, // 默认为翻页模式
            // 按键映射开关状态
            usbMappingEnabled: true,
            bleMappingEnabled: true,
            // 自定义映射状态
            customMappingEnabled: false,
            customLeftAction:  { action_type: 0, modifier: 0, keycode: 0x4B, text: '' },
            customRightAction: { action_type: 0, modifier: 0, keycode: 0x4E, text: '' },
            // WIFI状态
            wifiSSID: "",
            wifiIP: "",
            wifiRSSI: 0,
            wifiConnected: false
        };

        this.chainBusState = {
            leftBus: {
                connected: false,
                deviceCount: 0,
                refreshTime: '--:--:--',
                events: []
            },
            rightBus: {
                connected: false,
                deviceCount: 0,
                refreshTime: '--:--:--',
                events: []
            }
        };

        this.updateDisplay();
    }

    connectWebSocket() {
        try {
            const protocol = window.location.protocol === 'https:' ? 'wss:' : 'ws:';
            const wsUrl = `${protocol}//${window.location.host}/ws`;
            
            this.websocket = new WebSocket(wsUrl);
            
            this.websocket.onopen = () => {
                console.log('WebSocket连接成功');
                this.reconnectAttempts = 0;
                this.updateConnectionStatus(true);
                
                // 启动心跳检测
                this.startHeartbeat();
                
                // 请求初始状态
                this.sendMessage({
                    type: 'get_status'
                });
            };

            this.websocket.onmessage = (event) => {
                try {
                    const data = JSON.parse(event.data);
                    this.handleWebSocketMessage(data);
                } catch (error) {
                    console.error('解析WebSocket消息失败:', error);
                }
            };

            this.websocket.onclose = () => {
                console.log('WebSocket连接关闭');
                this.updateConnectionStatus(false);
                this.stopHeartbeat();
                this.attemptReconnect();
            };

            this.websocket.onerror = (error) => {
                console.error('WebSocket错误:', error);
                this.updateConnectionStatus(false);
                this.stopHeartbeat();
            };

        } catch (error) {
            console.error('WebSocket连接失败:', error);
            this.updateConnectionStatus(false);
            this.attemptReconnect();
        }
    }

    attemptReconnect() {
        this.reconnectAttempts++;
        console.log(`尝试重连 ${this.reconnectAttempts}...`);
        
        setTimeout(() => {
            this.connectWebSocket();
        }, this.reconnectDelay * this.reconnectAttempts);
    }

    sendMessage(message) {
        if (this.websocket && this.websocket.readyState === WebSocket.OPEN) {
            this.websocket.send(JSON.stringify(message));
        } else {
            console.warn('WebSocket未连接，无法发送消息:', message);
        }
    }

    // 修复handleWebSocketMessage方法，确保设备信息正确显示
    handleWebSocketMessage(data) {
        // console.log('收到WebSocket消息:', data);
        
        // 更新最后数据接收时间
        this.lastDataTime = Date.now();
        
        if (data.type === 'status_update') {
            // console.log('处理状态更新消息');
            
            // 更新DualKey状态
            if (data.dualkey) {
                // console.log('更新DualKey状态:', data.dualkey);
                this.dualkeyState.leftKey = data.dualkey.left_key_pressed || false;
                this.dualkeyState.rightKey = data.dualkey.right_key_pressed || false;
                this.dualkeyState.leftKeyColor = data.dualkey.left_key_color || 0x000000;
                this.dualkeyState.rightKeyColor = data.dualkey.right_key_color || 0x000000;
                this.dualkeyState.usbVoltage = data.dualkey.usb_voltage || 0.0;
                this.dualkeyState.usbConnected = data.dualkey.usb_connected || false;
                this.dualkeyState.usbMode = data.dualkey.usb_mode || 0;
                this.dualkeyState.batteryVoltage = data.dualkey.battery_voltage || 0;
                this.dualkeyState.batteryPercentage = data.dualkey.battery_percentage || 0;
                this.dualkeyState.charge_status = data.dualkey.charge_status || 0;
                this.dualkeyState.dipSwitch = data.dualkey.dip_switch_pos || 0;
                this.dualkeyState.switch1Value = data.dualkey.switch_1_value || 0;
                this.dualkeyState.switch2Value = data.dualkey.switch_2_value || 0;
                
                // 更新蓝牙状态
                this.dualkeyState.bluetoothConnected = data.dualkey.bluetooth_connected || false;
                this.dualkeyState.bluetoothDeviceName = data.dualkey.bluetooth_device_name || "Chain DualKey";
                this.dualkeyState.bluetoothPairingStatus = data.dualkey.bluetooth_pairing_status || 0;
                this.dualkeyState.bluetoothAdvStatus = data.dualkey.bluetooth_adv_status || false;
                
                // 更新按键映射
                if (data.dualkey.current_key_mapping !== undefined) {
                    this.dualkeyState.currentKeyMapping = data.dualkey.current_key_mapping;
                }
                
                // 更新按键映射开关状态
                if (data.dualkey.usb_mapping_enabled !== undefined) {
                    this.dualkeyState.usbMappingEnabled = data.dualkey.usb_mapping_enabled;
                }
                if (data.dualkey.ble_mapping_enabled !== undefined) {
                    this.dualkeyState.bleMappingEnabled = data.dualkey.ble_mapping_enabled;
                }

                // 更新自定义映射状态
                if (data.dualkey.custom_mapping_enabled !== undefined) {
                    this.dualkeyState.customMappingEnabled = data.dualkey.custom_mapping_enabled;
                }
                if (data.dualkey.custom_left_action !== undefined) {
                    this.dualkeyState.customLeftAction = data.dualkey.custom_left_action;
                }
                if (data.dualkey.custom_right_action !== undefined) {
                    this.dualkeyState.customRightAction = data.dualkey.custom_right_action;
                }

                // 更新WIFI状态
                if (data.dualkey.wifi_ssid !== undefined) {
                    this.dualkeyState.wifiSSID = data.dualkey.wifi_ssid;
                }
                if (data.dualkey.wifi_ip !== undefined) {
                    this.dualkeyState.wifiIP = data.dualkey.wifi_ip;
                }
                if (data.dualkey.wifi_rssi !== undefined) {
                    this.dualkeyState.wifiRSSI = data.dualkey.wifi_rssi;
                }
                if (data.dualkey.wifi_connected !== undefined) {
                    this.dualkeyState.wifiConnected = data.dualkey.wifi_connected;
                }
            }
            
            // 更新Chain Bus状态和设备显示
            if (data.chainbus) {
                // console.log('更新Chain Bus状态:', data.chainbus);
                
                // 更新基本状态
                if (data.chainbus.left_bus) {
                    this.chainBusState.leftBus.connected = data.chainbus.left_bus.connected || false;
                    this.chainBusState.leftBus.deviceCount = data.chainbus.left_bus.device_count || 0;
                    const now = new Date();
                    this.chainBusState.leftBus.refreshTime = now.toTimeString().split(' ')[0];
                    // console.log('左Bus状态:', this.chainBusState.leftBus);
                }
                if (data.chainbus.right_bus) {
                    this.chainBusState.rightBus.connected = data.chainbus.right_bus.connected || false;
                    this.chainBusState.rightBus.deviceCount = data.chainbus.right_bus.device_count || 0;
                    const now = new Date();
                    this.chainBusState.rightBus.refreshTime = now.toTimeString().split(' ')[0];
                    // console.log('右Bus状态:', this.chainBusState.rightBus);
                }
                
                // 调用设备显示更新函数
                this.updateChainBusDisplay(data.chainbus);
            }
            
            this.updateDisplay();
        }
    }

    updateConnectionStatus(connected) {
        const statusDot = document.querySelector('.status-dot');
        const statusText = document.querySelector('.status-text');
        
        if (connected) {
            statusDot.classList.add('online');
            statusDot.classList.remove('offline');
            statusText.textContent = languageManager.getText('online');
            // console.log('连接状态: 在线');
        } else {
            statusDot.classList.remove('online');
            statusDot.classList.add('offline');
            statusText.textContent = languageManager.getText('offline');
            // console.log('连接状态: 离线');
        }
        
        // 更新WebSocket状态显示
        const wsStatus = document.getElementById('ws-status');
        if (wsStatus) {
            wsStatus.textContent = connected ? languageManager.getText('connected') : languageManager.getText('disconnected');
            wsStatus.style.color = connected ? '#10b981' : '#ef4444';
        }
    }

    setupEventListeners() {
        // RGB 颜色控制
        const leftKeyColor = document.getElementById('leftKeyColor');
        const rightKeyColor = document.getElementById('rightKeyColor');
        
        if (leftKeyColor) {
            // leftKeyColor.addEventListener('change', (e) => {
            //     console.log('左键颜色变化事件触发:', e.target.value);
            //     const color = this.hexToRgbInt(e.target.value);
            //     this.updateKeyColor('left', color);
            // });
            
            leftKeyColor.addEventListener('input', (e) => {
                // console.log('左键颜色input事件触发:', e.target.value);
                const color = this.hexToRgbInt(e.target.value);
                this.updateKeyColor('left', color);
            });
        } else {
            console.error('未找到左键颜色选择器元素');
        }
        
        if (rightKeyColor) {
            // rightKeyColor.addEventListener('change', (e) => {
            //     console.log('右键颜色变化事件触发:', e.target.value);
            //     const color = this.hexToRgbInt(e.target.value);
            //     this.updateKeyColor('right', color);
            // });
            
            rightKeyColor.addEventListener('input', (e) => {
                // console.log('右键颜色input事件触发:', e.target.value);
                const color = this.hexToRgbInt(e.target.value);
                this.updateKeyColor('right', color);
            });
        } else {
            console.error('未找到右键颜色选择器元素');
        }

        // Bus 枚举设备按钮
        const enumerateLeftBus = document.getElementById('enumerateLeftBus');
        const enumerateRightBus = document.getElementById('enumerateRightBus');
        
        if (enumerateLeftBus) {
            enumerateLeftBus.addEventListener('click', () => {
                this.enumerateBusDevices('left');
            });
        }
        
        if (enumerateRightBus) {
            enumerateRightBus.addEventListener('click', () => {
                this.enumerateBusDevices('right');
            });
        }

        // 总线RGB设置
        const leftBusColorPicker = document.getElementById('leftBusColorPicker');
        const leftBusRgbBtn = document.getElementById('leftBusRgbBtn');
        const rightBusColorPicker = document.getElementById('rightBusColorPicker');
        const rightBusRgbBtn = document.getElementById('rightBusRgbBtn');
        
        if (leftBusColorPicker && leftBusRgbBtn) {
            leftBusRgbBtn.addEventListener('click', () => {
                const color = this.hexToRgbInt(leftBusColorPicker.value);
                this.setChainBusRGB('left', color);
            });
        }
        
        if (rightBusColorPicker && rightBusRgbBtn) {
            rightBusRgbBtn.addEventListener('click', () => {
                const color = this.hexToRgbInt(rightBusColorPicker.value);
                this.setChainBusRGB('right', color);
            });
        }

        // 按键模拟
        this.setupKeySimulation();
        
        // HID按键映射控制
        this.setupHIDMappingControls();
        
        // 蓝牙控制
        this.setupBluetoothControls();
        
        // WiFi配置控制
        this.setupWifiConfigControls();
        
        // 添加元素状态检查
        this.debugElementStatus();
        
        // console.log('事件监听器设置完成');
    }
    
    debugElementStatus() {
        // console.log('=== 元素状态检查 ===');
        
        const elements = {
            'leftKeyColor': document.getElementById('leftKeyColor'),
            'rightKeyColor': document.getElementById('rightKeyColor'),
            'enumerateLeftBus': document.getElementById('enumerateLeftBus'),
            'enumerateRightBus': document.getElementById('enumerateRightBus'),
            'leftBusColorPicker': document.getElementById('leftBusColorPicker'),
            'leftBusRgbBtn': document.getElementById('leftBusRgbBtn'),
            'rightBusColorPicker': document.getElementById('rightBusColorPicker'),
            'rightBusRgbBtn': document.getElementById('rightBusRgbBtn'),
            'leftKeyIndicator': document.querySelector('.left-key .key-indicator'),
            'rightKeyIndicator': document.querySelector('.right-key .key-indicator')
        };
        
        for (const [name, element] of Object.entries(elements)) {
            if (element) {
                // console.log(`✓ ${name}: 已找到`, element);
                if (name.includes('Color')) {
                    // console.log(`  - 当前值: ${element.value}`);
                    // console.log(`  - 类型: ${element.type}`);
                }
            } else {
                console.warn(`✗ ${name}: 未找到`);
            }
        }
        
        // console.log('=== 元素状态检查完成 ===');
    }

    hexToRgbInt(hex) {
        // 将 #RRGGBB 转换为整数
        const result = parseInt(hex.replace('#', ''), 16);
        return result;
    }

    rgbIntToHex(rgbInt) {
        // 将整数转换为 #RRGGBB
        return '#' + ('000000' + rgbInt.toString(16)).slice(-6);
    }

    formatUpdateTime(timestamp) {
        if (!timestamp || timestamp === 0) {
            return '--:--:--';
        }
        
        // timestamp是毫秒，转换为Date对象
        const date = new Date(timestamp);
        
        // 格式化为 HH:MM:SS
        return date.toLocaleTimeString('zh-CN', {
            hour12: false,
            hour: '2-digit',
            minute: '2-digit',
            second: '2-digit'
        });
    }

    formatDeviceUID(uid) {
        if (!uid || !Array.isArray(uid) || uid.length === 0) {
            return 'NULL';
        }
        
        // 将UID数组转换为十六进制字符串
        return uid.map(byte => byte.toString(16).padStart(2, '0')).join('').toUpperCase();
    }
    formatEventDisplay(device) {
        if (!device.last_event) {
            return '';
        }
        
        // 使用事件触发时间，如果没有则使用当前时间作为后备
        const eventTime = this.formatUpdateTime(device.event_trigger_time || Date.now());
        return `<span class="last-event">${languageManager.getText('event')}: ${device.last_event} (${eventTime})</span>`;
    }

    setupKeySimulation() {
        // 模拟按键按下
        document.addEventListener('keydown', (e) => {
            if (e.code === 'PageUp') {
                this.simulateKeyPress('left');
            } else if (e.code === 'PageDown') {
                this.simulateKeyPress('right');
            }
        });

        // 模拟按键释放
        document.addEventListener('keyup', (e) => {
            if (e.code === 'PageUp') {
                this.simulateKeyRelease('left');
            } else if (e.code === 'PageDown') {
                this.simulateKeyRelease('right');
            }
        });
    }

    // HID按键映射控制
    setupHIDMappingControls() {
        const hidMappingSelect = document.getElementById('hidMappingSelect');
        const applyMappingBtn  = document.getElementById('applyMappingBtn');
        const usbMappingSwitch = document.getElementById('usbMappingSwitch');
        const bleMappingSwitch = document.getElementById('bleMappingSwitch');

        // ---- 动态填充键码下拉选项 ----
        ['leftKeyCodeSelect', 'rightKeyCodeSelect'].forEach(id => {
            const sel = document.getElementById(id);
            if (!sel) return;
            sel.innerHTML = '';
            this.keyCodeOptions.forEach(opt => {
                const el = document.createElement('option');
                el.value       = opt.value;
                el.textContent = opt.label;
                sel.appendChild(el);
            });
        });

        // ---- 映射类型标签切换 ----
        ['tabPresetMapping', 'tabCustomKeyMapping', 'tabCustomTextMapping'].forEach(id => {
            const btn = document.getElementById(id);
            if (!btn) return;
            btn.addEventListener('click', () => {
                this.switchMappingTab(btn.dataset.tab);
            });
        });

        // ---- 预设映射 ----
        if (hidMappingSelect) {
            hidMappingSelect.value = this.dualkeyState.currentKeyMapping;
        }
        if (applyMappingBtn) {
            applyMappingBtn.addEventListener('click', () => {
                this.applyHIDMapping(parseInt(hidMappingSelect.value));
            });
        }

        // ---- 自定义按键：应用 ----
        const applyCustomKeyBtn = document.getElementById('applyCustomKeyBtn');
        if (applyCustomKeyBtn) {
            applyCustomKeyBtn.addEventListener('click', () => {
                this.applyCustomKeyMapping();
            });
        }

        // ---- 自定义文本：应用 ----
        const applyCustomTextBtn = document.getElementById('applyCustomTextBtn');
        if (applyCustomTextBtn) {
            applyCustomTextBtn.addEventListener('click', () => {
                this.applyCustomTextMapping();
            });
        }

        // USB映射开关
        if (usbMappingSwitch) {
            usbMappingSwitch.checked = this.dualkeyState.usbMappingEnabled;
            usbMappingSwitch.addEventListener('change', () => {
                this.setKeyMappingSwitch('usb', usbMappingSwitch.checked);
            });
        }

        // 蓝牙映射开关
        if (bleMappingSwitch) {
            bleMappingSwitch.checked = this.dualkeyState.bleMappingEnabled;
            bleMappingSwitch.addEventListener('change', () => {
                this.setKeyMappingSwitch('ble', bleMappingSwitch.checked);
            });
        }
    }

    // 切换映射标签
    switchMappingTab(tab) {
        this.currentMappingTab = tab;

        // 更新标签按钮状态
        ['tabPresetMapping', 'tabCustomKeyMapping', 'tabCustomTextMapping'].forEach(id => {
            const btn = document.getElementById(id);
            if (btn) btn.classList.toggle('active', btn.dataset.tab === tab);
        });

        // 显示/隐藏对应面板
        const panels = {
            preset:    'panelPresetMapping',
            customKey: 'panelCustomKeyMapping',
            customText:'panelCustomTextMapping',
        };
        Object.entries(panels).forEach(([key, panelId]) => {
            const el = document.getElementById(panelId);
            if (el) el.classList.toggle('hidden', key !== tab);
        });
    }

    // 应用自定义按键 / 组合键
    applyCustomKeyMapping() {
        const getModifier = (idCtrl, idShift, idAlt, idGui) => {
            let mod = 0;
            if (document.getElementById(idCtrl)  ?.checked) mod |= 0x01;
            if (document.getElementById(idShift) ?.checked) mod |= 0x02;
            if (document.getElementById(idAlt)   ?.checked) mod |= 0x04;
            if (document.getElementById(idGui)   ?.checked) mod |= 0x08;
            return mod;
        };
        const leftMod  = getModifier('leftModCtrl',  'leftModShift',  'leftModAlt',  'leftModGui');
        const rightMod = getModifier('rightModCtrl', 'rightModShift', 'rightModAlt', 'rightModGui');
        const leftKc   = parseInt(document.getElementById('leftKeyCodeSelect') ?.value  || '0');
        const rightKc  = parseInt(document.getElementById('rightKeyCodeSelect')?.value || '0');

        const leftAction  = { action_type: 0, modifier: leftMod,  keycode: leftKc,  text: '' };
        const rightAction = { action_type: 0, modifier: rightMod, keycode: rightKc, text: '' };

        this.sendMessage({
            type: 'set_custom_mapping',
            enabled: true,
            left_key:  leftAction,
            right_key: rightAction,
        });

        // 同步本地状态
        this.dualkeyState.customMappingEnabled = true;
        this.dualkeyState.customLeftAction     = leftAction;
        this.dualkeyState.customRightAction    = rightAction;
        this.hidStatusCache.customMappingEnabled = true;
        this.hidStatusCache.customLeftAction     = leftAction;
        this.hidStatusCache.customRightAction    = rightAction;

        this.updateMappingStatusDisplay();

        const btn = document.getElementById('applyCustomKeyBtn');
        if (btn) {
            btn.disabled = true;
            btn.textContent = languageManager.getText('applying');
            setTimeout(() => { btn.disabled = false; btn.textContent = languageManager.getText('apply'); }, 2000);
        }
    }

    // 应用自定义文本
    applyCustomTextMapping() {
        const leftText  = (document.getElementById('leftKeyText') ?.value  || '').substring(0, 63);
        const rightText = (document.getElementById('rightKeyText')?.value || '').substring(0, 63);

        const leftAction  = { action_type: 1, modifier: 0, keycode: 0, text: leftText };
        const rightAction = { action_type: 1, modifier: 0, keycode: 0, text: rightText };

        this.sendMessage({
            type: 'set_custom_mapping',
            enabled: true,
            left_key:  leftAction,
            right_key: rightAction,
        });

        // 同步本地状态
        this.dualkeyState.customMappingEnabled = true;
        this.dualkeyState.customLeftAction     = leftAction;
        this.dualkeyState.customRightAction    = rightAction;
        this.hidStatusCache.customMappingEnabled = true;
        this.hidStatusCache.customLeftAction     = leftAction;
        this.hidStatusCache.customRightAction    = rightAction;

        this.updateMappingStatusDisplay();

        const btn = document.getElementById('applyCustomTextBtn');
        if (btn) {
            btn.disabled = true;
            btn.textContent = languageManager.getText('applying');
            setTimeout(() => { btn.disabled = false; btn.textContent = languageManager.getText('apply'); }, 2000);
        }
    }

    // 从设备状态恢复自定义映射UI（设备重连后同步）
    restoreCustomMappingUI(dualkey) {
        if (!dualkey) return;

        const customEnabled = dualkey.custom_mapping_enabled;
        const leftAction    = dualkey.custom_left_action;
        const rightAction   = dualkey.custom_right_action;

        if (customEnabled === undefined) return;

        this.hidStatusCache.customMappingEnabled = customEnabled;

        if (!customEnabled) {
            // 自定义映射未启用：切回预设标签
            if (this.currentMappingTab !== 'preset') {
                this.switchMappingTab('preset');
            }
            this.updateMappingStatusDisplay();
            return;
        }

        if (leftAction && rightAction) {
            const actionType = leftAction.action_type;
            const tab = actionType === 1 ? 'customText' : 'customKey';

            // 只在当前仍是预设标签时才自动切换，避免覆盖用户正在操作的状态
            if (this.currentMappingTab === 'preset') {
                this.switchMappingTab(tab);
            }

            if (actionType === 0) {
                // 恢复按键 / 组合键设置
                const restoreKey = (idCtrl, idShift, idAlt, idGui, idSel, action) => {
                    const el = (id) => document.getElementById(id);
                    if (el(idCtrl))  el(idCtrl).checked  = !!(action.modifier & 0x01);
                    if (el(idShift)) el(idShift).checked = !!(action.modifier & 0x02);
                    if (el(idAlt))   el(idAlt).checked   = !!(action.modifier & 0x04);
                    if (el(idGui))   el(idGui).checked   = !!(action.modifier & 0x08);
                    if (el(idSel))   el(idSel).value     = action.keycode;
                };
                restoreKey('leftModCtrl',  'leftModShift',  'leftModAlt',  'leftModGui',  'leftKeyCodeSelect',  leftAction);
                restoreKey('rightModCtrl', 'rightModShift', 'rightModAlt', 'rightModGui', 'rightKeyCodeSelect', rightAction);
            } else {
                // 恢复文本设置
                const lt = document.getElementById('leftKeyText');
                const rt = document.getElementById('rightKeyText');
                if (lt) lt.value = leftAction.text  || '';
                if (rt) rt.value = rightAction.text || '';
            }
        }

        this.updateMappingStatusDisplay();
    }

    // 蓝牙控制
    setupBluetoothControls() {
        const bleToggleAdvBtn = document.getElementById('bleToggleAdvBtn');
        const blePairingBtn = document.getElementById('blePairingBtn');
        
        if (bleToggleAdvBtn) {
            bleToggleAdvBtn.addEventListener('click', () => {
                // console.log('切换蓝牙广播状态');
                this.toggleBluetoothAdvertising();
            });
        }
        
        if (blePairingBtn) {
            blePairingBtn.addEventListener('click', () => {
                // 显示配对提示弹窗
                this.showPairingDialog();
                // console.log('进入配对模式');
                this.startBluetoothPairing();
            });
        }
    }

    // WiFi配置控制
    setupWifiConfigControls() {
        const staticIPSwitch = document.getElementById('wifiStaticIPSwitch');
        const staticIPConfig = document.getElementById('wifiStaticIPConfig');
        const applyBtn = document.getElementById('wifiConfigApplyBtn');
        const resetBtn = document.getElementById('wifiConfigResetBtn');
        
        // 静态IP开关
        if (staticIPSwitch && staticIPConfig) {
            staticIPSwitch.addEventListener('change', () => {
                if (staticIPSwitch.checked) {
                    staticIPConfig.classList.remove('hidden');
                } else {
                    staticIPConfig.classList.add('hidden');
                }
            });
        }
        
        // 应用配置按钮
        if (applyBtn) {
            applyBtn.addEventListener('click', () => {
                this.applyWifiConfig();
            });
        }
        
        // 重置配置按钮
        if (resetBtn) {
            resetBtn.addEventListener('click', () => {
                if (confirm(languageManager.getText('confirmResetWifi'))) {
                    this.resetWifiConfig();
                }
            });
        }
    }

    // 应用WiFi配置
    applyWifiConfig() {
        const ssid = document.getElementById('wifiConfigSSID').value;
        const password = document.getElementById('wifiConfigPassword').value;
        const useStaticIP = document.getElementById('wifiStaticIPSwitch').checked;
        
        if (!ssid || ssid.trim() === '') {
            alert(languageManager.getText('enterWifiSSID'));
            return;
        }
        
        if (!password || password.trim() === '') {
            alert(languageManager.getText('enterWifiPassword'));
            return;
        }
        
        const config = {
            ssid: ssid,
            password: password,
            use_static_ip: useStaticIP
        };
        
        if (useStaticIP) {
            const staticIP = document.getElementById('wifiStaticIP').value;
            const netmask = document.getElementById('wifiStaticNetmask').value;
            const gateway = document.getElementById('wifiStaticGateway').value;
            
            if (!staticIP || !netmask || !gateway) {
                alert(languageManager.getText('enterStaticIPConfig'));
                return;
            }
            
            config.static_ip = staticIP;
            config.netmask = netmask;
            config.gateway = gateway;
        }
        
        console.log('应用WiFi配置:', config);
        
        // 发送配置到后端
        this.sendMessage({
            type: 'set_wifi_config',
            config: config
        });
        
        // 禁用按钮
        const applyBtn = document.getElementById('wifiConfigApplyBtn');
        if (applyBtn) {
            applyBtn.disabled = true;
            applyBtn.textContent = languageManager.getText('configuring');
            setTimeout(() => {
                applyBtn.disabled = false;
                applyBtn.textContent = languageManager.getText('applyConfig');
                alert(languageManager.getText('wifiConfigSaved'));
            }, 3000);
        }
    }

    // 重置WiFi配置
    resetWifiConfig() {
        console.log('重置WiFi配置');
        
        // 发送重置命令到后端
        this.sendMessage({
            type: 'reset_wifi_config'
        });
        
        // 清空输入框
        document.getElementById('wifiConfigSSID').value = '';
        document.getElementById('wifiConfigPassword').value = '';
        document.getElementById('wifiStaticIPSwitch').checked = false;
        document.getElementById('wifiStaticIPConfig').classList.add('hidden');
        document.getElementById('wifiStaticIP').value = '';
        document.getElementById('wifiStaticNetmask').value = '';
        document.getElementById('wifiStaticGateway').value = '';
        
        // 禁用按钮
        const resetBtn = document.getElementById('wifiConfigResetBtn');
        if (resetBtn) {
            resetBtn.disabled = true;
            resetBtn.textContent = languageManager.getText('configuring');
            setTimeout(() => {
                resetBtn.disabled = false;
                resetBtn.textContent = languageManager.getText('resetConfig');
            }, 3000);
        }
    }

    // 应用HID按键映射
    applyHIDMapping(mappingIndex) {
        // 发送预设映射索引
        this.sendMessage({
            type: 'set_hid_mapping',
            mapping_index: mappingIndex
        });

        // 同时禁用自定义映射，确保预设模式真正生效
        // 不传 left_key/right_key，设备端保留现有的自定义动作值，方便用户切回时复用
        this.sendMessage({
            type: 'set_custom_mapping',
            enabled: false,
        });

        // 同步本地状态
        this.dualkeyState.currentKeyMapping   = mappingIndex;
        this.dualkeyState.customMappingEnabled = false;
        this.hidStatusCache.customMappingEnabled = false;

        // 切换到预设标签并刷新状态显示
        this.switchMappingTab('preset');
        this.updateMappingStatusDisplay();

        const applyBtn = document.getElementById('applyMappingBtn');
        if (applyBtn) {
            applyBtn.disabled = true;
            applyBtn.textContent = languageManager.getText('applying');
            setTimeout(() => {
                applyBtn.disabled = false;
                applyBtn.textContent = languageManager.getText('apply');
            }, 2000);
        }
    }

    // 切换蓝牙广播状态
    toggleBluetoothAdvertising() {
        // console.log('切换蓝牙广播状态');
        
        // 根据当前广播状态决定操作
        if (this.dualkeyState.bluetoothAdvStatus) {
            // 如果正在广播，则停止广播
            this.sendMessage({
                type: 'bluetooth_disconnect'
            });
        } else {
            // 如果未广播，则开始广播
            this.sendMessage({
                type: 'bluetooth_start_adv'
            });
        }
        
        // 更新按钮状态
        const toggleBtn = document.getElementById('bleToggleAdvBtn');
        if (toggleBtn) {
            toggleBtn.disabled = true;
            if (this.dualkeyState.bluetoothAdvStatus) {
                toggleBtn.textContent = languageManager.getText('stopping');
            } else {
                toggleBtn.textContent = languageManager.getText('starting');
            }
            setTimeout(() => {
                toggleBtn.disabled = false;
                this.updateBluetoothButtonText();
            }, 5000);
        }
    }

    // 开始蓝牙配对
    startBluetoothPairing() {
        // console.log('开始蓝牙配对');
        
        // 发送到后端
        this.sendMessage({
            type: 'bluetooth_start_pairing'
        });
        
        // 更新按钮状态
        const pairingBtn = document.getElementById('blePairingBtn');
        if (pairingBtn) {
            pairingBtn.disabled = true;
            pairingBtn.textContent = languageManager.getText('pairing');
            setTimeout(() => {
                pairingBtn.disabled = false;
                pairingBtn.textContent = languageManager.getText('pairingMode');
            }, 10000); // 配对模式持续10秒
        }
    }

    // 更新蓝牙按钮文本
    updateBluetoothButtonText() {
        const toggleBtn = document.getElementById('bleToggleAdvBtn');
        if (toggleBtn) {
            if (this.dualkeyState.bluetoothAdvStatus) {
                toggleBtn.textContent = languageManager.getText('stopBroadcast');
            } else {
                toggleBtn.textContent = languageManager.getText('startBroadcast');
            }
        }
    }

    // 设置按键映射开关
    setKeyMappingSwitch(type, enabled) {
        // console.log(`设置${type}映射开关: ${enabled ? '启用' : '禁用'}`);
        
        // 发送到后端
        this.sendMessage({
            type: 'set_key_mapping_switch',
            mapping_type: type,
            enabled: enabled
        });
        
        // 更新本地状态
        if (type === 'usb') {
            this.dualkeyState.usbMappingEnabled = enabled;
        } else if (type === 'ble') {
            this.dualkeyState.bleMappingEnabled = enabled;
        }
        
        // 更新映射状态显示
        this.updateMappingStatusDisplay();
    }

    simulateKeyPress(key) {
        if (key === 'left') {
            this.dualkeyState.leftKey = true;
        } else if (key === 'right') {
            this.dualkeyState.rightKey = true;
        }
        this.updateDisplay();
    }

    simulateKeyRelease(key) {
        if (key === 'left') {
            this.dualkeyState.leftKey = false;
        } else if (key === 'right') {
            this.dualkeyState.rightKey = false;
        }
        this.updateDisplay();
    }

    updateKeyColor(key, color) {
        // console.log(`更新${key}键颜色: 0x${color.toString(16).padStart(6, '0').toUpperCase()}`);
        
        try {
            // 发送到后端
            this.sendMessage({
                type: 'set_rgb',
                key: key,
                color: color
            });
            
            // 立即更新显示
            if (key === 'left') {
                this.dualkeyState.leftKeyColor = color;
                const leftIndicator = document.querySelector('.left-key .key-indicator');
                if (leftIndicator) {
                    leftIndicator.style.backgroundColor = this.rgbIntToHex(color);
                    // console.log(`左键指示器颜色已更新为: ${this.rgbIntToHex(color)}`);
                } else {
                    console.warn('未找到左键指示器元素');
                }
            } else if (key === 'right') {
                this.dualkeyState.rightKeyColor = color;
                const rightIndicator = document.querySelector('.right-key .key-indicator');
                if (rightIndicator) {
                    rightIndicator.style.backgroundColor = this.rgbIntToHex(color);
                    // console.log(`右键指示器颜色已更新为: ${this.rgbIntToHex(color)}`);
                } else {
                    console.warn('未找到右键指示器元素');
                }
            }
            
            // console.log(`${key}键颜色更新完成`);
        } catch (error) {
            console.error(`更新${key}键颜色时发生错误:`, error);
        }
    }

    enumerateBusDevices(bus) {
        // console.log(`枚举${bus} Bus设备`);
        
        // 发送枚举请求到后端
        this.sendMessage({
            type: 'enumerate_bus',
            bus: bus
        });
        
        // 添加本地事件日志
        const now = new Date();
        const timeString = now.toTimeString().split(' ')[0];
        this.addBusEvent(bus, '开始枚举设备');
        
        // 更新刷新时间
        if (bus === 'left') {
            this.chainBusState.leftBus.refreshTime = timeString;
        } else if (bus === 'right') {
            this.chainBusState.rightBus.refreshTime = timeString;
        }
    }

    addBusEvent(bus, eventType) {
        const busState = this.chainBusState[bus + 'Bus'];
        const now = new Date();
        const timeString = now.toTimeString().split(' ')[0];
        
        busState.events.unshift({
            time: timeString,
            type: eventType
        });
        
        // 保持最多5个事件
        if (busState.events.length > 5) {
            busState.events.pop();
        }
        
        this.updateBusEvents(bus);
    }

    updateBusEvents(bus) {
        const busElement = document.querySelector(`.${bus}-bus .event-list`);
        if (!busElement) return;
        
        const busState = this.chainBusState[bus + 'Bus'];
        busElement.innerHTML = '';
        
        busState.events.forEach(event => {
            const eventElement = document.createElement('div');
            eventElement.className = 'event-item';
            eventElement.innerHTML = `
                <span class="event-time">${event.time}</span>
                <span class="event-type">${event.type}</span>
            `;
            busElement.appendChild(eventElement);
        });
    }

    updateDisplay() {
        // 更新按键状态
        this.updateKeyDisplay();
        
        // 更新拨码开关
        this.updateDipSwitchDisplay();
        
        // 更新HID设备状态（合并了USB、蓝牙和按键映射）
        this.updateHIDStatus();
        
        // 注意：Bus状态由updateBusChainDisplay函数单独处理，不在此处重复更新
        // Chain Bus的设备数量和刷新时间由WebSocket消息触发的updateBusChainDisplay处理
        
        // 更新电量显示
        this.updateBatteryDisplay();
        
        // 更新WIFI状态显示
        this.updateWifiDisplay();
        
        // 更新最后更新时间
        this.updateLastUpdate();
        
        // 更新颜色选择器的值
        this.updateColorPickers();
    }

    updateKeyDisplay() {
        // 左键
        const leftKeyIndicator = document.querySelector('.left-key .key-indicator');
        if (this.dualkeyState.leftKey) {
            leftKeyIndicator.classList.add('active');
        } else {
            leftKeyIndicator.classList.remove('active');
        }
        leftKeyIndicator.style.backgroundColor = this.rgbIntToHex(this.dualkeyState.leftKeyColor);
        
        // 右键
        const rightKeyIndicator = document.querySelector('.right-key .key-indicator');
        if (this.dualkeyState.rightKey) {
            rightKeyIndicator.classList.add('active');
        } else {
            rightKeyIndicator.classList.remove('active');
        }
        rightKeyIndicator.style.backgroundColor = this.rgbIntToHex(this.dualkeyState.rightKeyColor);
    }

    updateColorPickers() {
        // 更新颜色选择器的值，但不触发事件
        const leftColorPicker = document.getElementById('leftKeyColor');
        const rightColorPicker = document.getElementById('rightKeyColor');
        
        leftColorPicker.value = this.rgbIntToHex(this.dualkeyState.leftKeyColor);
        rightColorPicker.value = this.rgbIntToHex(this.dualkeyState.rightKeyColor);
    }

    updateDipSwitchDisplay() {
        const slider = document.querySelector('.dip-switch-slider');
        const labels = document.querySelectorAll('.switch-label');
        
        // 移除所有状态类
        slider.classList.remove('left', 'center', 'right');
        labels.forEach(label => label.classList.remove('active'));
        
        // 根据数值设置位置
        let position = 'center';
        switch (this.dualkeyState.dipSwitch) {
            case 0:
                position = 'center';
                break;
            case 1:
                position = 'left';
                break;
            case 2:
                position = 'right';
                break;
        }
        
        // 设置滑块位置和颜色
        slider.classList.add(position);
        
        // 高亮当前状态的标签
        const activeLabel = document.querySelector(`.switch-label.${position}`);
        if (activeLabel) {
            activeLabel.classList.add('active');
        }
    }

    // 更新HID设备状态 - 使用缓存机制避免频繁DOM更新
    updateHIDStatus() {
        // USB连接状态
        if (this.hidStatusCache.usbConnected !== this.dualkeyState.usbConnected) {
            const usbConnectionStatus = document.getElementById('usbConnectionStatus');
            if (usbConnectionStatus) {
                if (this.dualkeyState.usbConnected) {
                    usbConnectionStatus.textContent = languageManager.getText('connected');
                    usbConnectionStatus.className = 'value connected';
                } else {
                    usbConnectionStatus.textContent = languageManager.getText('notConnected');
                    usbConnectionStatus.className = 'value';
                }
            }
            this.hidStatusCache.usbConnected = this.dualkeyState.usbConnected;
        }

        // 蓝牙连接状态
        if (this.hidStatusCache.bluetoothConnected !== this.dualkeyState.bluetoothConnected) {
            const bleConnectionStatus = document.getElementById('bleConnectionStatus');
            if (bleConnectionStatus) {
                if (this.dualkeyState.bluetoothConnected) {
                    bleConnectionStatus.textContent = languageManager.getText('connected');
                    bleConnectionStatus.className = 'value connected';
                } else {
                    bleConnectionStatus.textContent = languageManager.getText('notConnected');
                    bleConnectionStatus.className = 'value';
                }
            }
            
            this.hidStatusCache.bluetoothConnected = this.dualkeyState.bluetoothConnected;
        }

        // 蓝牙广播状态
        if (this.hidStatusCache.bluetoothAdvStatus !== this.dualkeyState.bluetoothAdvStatus) {
            // 更新蓝牙按钮状态
            const bleToggleAdvBtn = document.getElementById('bleToggleAdvBtn');
            const blePairingBtn = document.getElementById('blePairingBtn');
            if (bleToggleAdvBtn) {
                bleToggleAdvBtn.disabled = false; // 始终可用，根据状态切换功能
                this.updateBluetoothButtonText();
            }
            if (blePairingBtn) {
                blePairingBtn.disabled = true;
            }
            
            this.hidStatusCache.bluetoothAdvStatus = this.dualkeyState.bluetoothAdvStatus;
        }

        // 设备名称
        if (this.hidStatusCache.deviceName !== this.dualkeyState.bluetoothDeviceName) {
            const deviceName = document.getElementById('deviceName');
            if (deviceName) {
                deviceName.textContent = this.dualkeyState.bluetoothDeviceName;
            }
            this.hidStatusCache.deviceName = this.dualkeyState.bluetoothDeviceName;
        }

        // 设备类型
        const modeNames = ['HID Keyboard', 'BLE Keyboard', 'USB CDC'];
        const currentDeviceType = modeNames[this.dualkeyState.usbMode] || 'Unknown';
        if (this.hidStatusCache.deviceType !== currentDeviceType) {
            const deviceType = document.getElementById('deviceType');
            if (deviceType) {
                deviceType.textContent = currentDeviceType;
            }
            this.hidStatusCache.deviceType = currentDeviceType;
        }

        // 按键映射 - 只在值变化时更新select，避免打断用户操作
        if (this.hidStatusCache.currentKeyMapping !== this.dualkeyState.currentKeyMapping) {
            const hidMappingSelect = document.getElementById('hidMappingSelect');
            if (hidMappingSelect && !hidMappingSelect.matches(':focus')) {
                // 只有在下拉框没有焦点时才更新，避免用户正在操作时被打断
                hidMappingSelect.value = this.dualkeyState.currentKeyMapping;
            }
            this.hidStatusCache.currentKeyMapping = this.dualkeyState.currentKeyMapping;
            this.updateMappingStatusDisplay();
            // console.log('HID按键映射已更新为:', this.dualkeyState.currentKeyMapping);
        }

        // USB映射开关
        if (this.hidStatusCache.usbMappingEnabled !== this.dualkeyState.usbMappingEnabled) {
            const usbMappingSwitch = document.getElementById('usbMappingSwitch');
            if (usbMappingSwitch) {
                usbMappingSwitch.checked = this.dualkeyState.usbMappingEnabled;
            }
            this.hidStatusCache.usbMappingEnabled = this.dualkeyState.usbMappingEnabled;
            this.updateMappingStatusDisplay();
        }

        // 蓝牙映射开关
        if (this.hidStatusCache.bleMappingEnabled !== this.dualkeyState.bleMappingEnabled) {
            const bleMappingSwitch = document.getElementById('bleMappingSwitch');
            if (bleMappingSwitch) {
                bleMappingSwitch.checked = this.dualkeyState.bleMappingEnabled;
            }
            this.hidStatusCache.bleMappingEnabled = this.dualkeyState.bleMappingEnabled;
            this.updateMappingStatusDisplay();
        }

        // 自定义映射：从设备状态同步 UI（首次连接 / 重连时恢复）
        const prevCustomEnabled = this.hidStatusCache.customMappingEnabled;
        if (this.dualkeyState.customMappingEnabled !== undefined &&
            prevCustomEnabled !== this.dualkeyState.customMappingEnabled) {
            this.restoreCustomMappingUI(this.dualkeyState);
            this.hidStatusCache.customMappingEnabled = this.dualkeyState.customMappingEnabled;
        }
    }

    // 更新映射状态显示
    updateMappingStatusDisplay() {
        const currentMappingName = document.getElementById('currentMappingName');
        const usbMappingDetail   = document.getElementById('usbMappingDetail');
        const bleMappingDetail   = document.getElementById('bleMappingDetail');

        // 从 keyCodeOptions 中查找键名
        const getKeyName = (keycode) => {
            const opt = this.keyCodeOptions.find(o => o.value === keycode);
            return opt ? opt.label : (keycode ? `0x${keycode.toString(16).toUpperCase()}` : '--');
        };

        // 将修饰键掩码转为可读字符串
        const getModStr = (modifier) => {
            const parts = [];
            if (modifier & 0x01) parts.push('Ctrl');
            if (modifier & 0x02) parts.push('Shift');
            if (modifier & 0x04) parts.push('Alt');
            if (modifier & 0x08) parts.push('GUI');
            return parts.join('+');
        };

        if (this.dualkeyState.customMappingEnabled) {
            const leftAction  = this.dualkeyState.customLeftAction  || {};
            const rightAction = this.dualkeyState.customRightAction || {};

            if (leftAction.action_type === 1) {
                // ---- 自定义文本模式 ----
                if (currentMappingName) {
                    currentMappingName.textContent = `[${languageManager.getText('customTextMapping')}]`;
                }
                const truncate = (str, len) => {
                    if (!str) return '""';
                    return str.length > len ? `"${str.substring(0, len)}…"` : `"${str}"`;
                };
                if (usbMappingDetail) {
                    usbMappingDetail.textContent = `L: ${truncate(leftAction.text, 18)}`;
                    usbMappingDetail.classList.remove('disabled');
                }
                if (bleMappingDetail) {
                    bleMappingDetail.textContent = `R: ${truncate(rightAction.text, 18)}`;
                    bleMappingDetail.classList.remove('disabled');
                }
            } else {
                // ---- 自定义按键模式 ----
                if (currentMappingName) {
                    currentMappingName.textContent = `[${languageManager.getText('customKeyMapping')}]`;
                }
                const formatKey = (action) => {
                    const modStr = getModStr(action.modifier || 0);
                    const keyStr = getKeyName(action.keycode || 0);
                    return modStr ? `${modStr}+${keyStr}` : keyStr;
                };
                if (usbMappingDetail) {
                    usbMappingDetail.textContent = `L: ${formatKey(leftAction)}`;
                    usbMappingDetail.classList.remove('disabled');
                }
                if (bleMappingDetail) {
                    bleMappingDetail.textContent = `R: ${formatKey(rightAction)}`;
                    bleMappingDetail.classList.remove('disabled');
                }
            }
        } else {
            // ---- 预设映射模式 ----
            const mappingNames = [
                languageManager.getText('copyPaste'),
                languageManager.getText('copyPasteCmd'),
                languageManager.getText('undoRedo'),
                languageManager.getText('undoRedoCmd'),
                languageManager.getText('undoRedoCmdShift'),
                languageManager.getText('tabSwitch'),
                languageManager.getText('windowSwitch'),
                languageManager.getText('windowSwitchCmd'),
                languageManager.getText('zoom'),
                languageManager.getText('zoomCmd'),
                languageManager.getText('pageUpDown'),
                languageManager.getText('volumeControl'),
                languageManager.getText('mediaControl'),
                languageManager.getText('mediaControlPlayPause'),
                languageManager.getText('homeEnd'),
                languageManager.getText('upDown'),
                languageManager.getText('leftRight'),
            ];
            const presetName = mappingNames[this.dualkeyState.currentKeyMapping] || mappingNames[10];
            if (currentMappingName) {
                currentMappingName.textContent = presetName;
            }

            if (usbMappingDetail) {
                const en = this.dualkeyState.usbMappingEnabled;
                usbMappingDetail.textContent = `USB: ${en ? languageManager.getText('enabled') : languageManager.getText('disabled')}`;
                usbMappingDetail.classList.toggle('disabled', !en);
            }
            if (bleMappingDetail) {
                const en = this.dualkeyState.bleMappingEnabled;
                bleMappingDetail.textContent = `BLE: ${en ? languageManager.getText('enabled') : languageManager.getText('disabled')}`;
                bleMappingDetail.classList.toggle('disabled', !en);
            }
        }
    }

    // updateBusDisplay函数已移除，所有Bus状态更新现在由updateBusChainDisplay处理

    updateBatteryDisplay() {
        const batteryLevel = document.querySelector('.battery-level');
        const batteryPercentage = document.querySelector('.battery-percentage');
        const chargingStatus = document.querySelector('.charging-status');
        
        batteryLevel.style.width = `${this.dualkeyState.batteryPercentage}%`;
        batteryPercentage.textContent = `${this.dualkeyState.batteryPercentage}%`;
        
        if (this.dualkeyState.charge_status == 1) {
            chargingStatus.textContent = languageManager.getText('charging');
            chargingStatus.className = 'charging-status charging';
        } else if (this.dualkeyState.charge_status == 2) {
            chargingStatus.textContent = languageManager.getText('fullyCharged');
            chargingStatus.className = 'charging-status charging';
        } else {
            chargingStatus.textContent = languageManager.getText('notCharging');
            chargingStatus.className = 'charging-status not-charging';
        }
        
        // 更新电压显示
        const batteryVoltageElement = document.getElementById('batteryVoltage');
        const usbVoltageElement = document.getElementById('usbVoltage');
        const switch1ValueElement = document.getElementById('switch1Value');
        const switch2ValueElement = document.getElementById('switch2Value');
        
        if (batteryVoltageElement) {
            batteryVoltageElement.textContent = `${this.dualkeyState.batteryVoltage.toFixed(2)}V`;
        }
        
        if (usbVoltageElement) {
            usbVoltageElement.textContent = `${this.dualkeyState.usbVoltage.toFixed(2)}V`;
        }

        if (switch1ValueElement) {
            switch1ValueElement.textContent = `${this.dualkeyState.switch1Value}mV`;
        }

        if (switch2ValueElement) {
            switch2ValueElement.textContent = `${this.dualkeyState.switch2Value}mV`;
        }
        
        // 根据电压调整电池颜色
        const voltage = this.dualkeyState.batteryVoltage;
        if (voltage < 3.2) {
            batteryLevel.style.background = '#ef4444'; // 红色 - 低电量
        } else if (voltage > 4.0) {
            batteryLevel.style.background = '#10b981'; // 绿色 - 电量充足
        } else {
            batteryLevel.style.background = 'linear-gradient(90deg, #ef4444, #f59e0b, #10b981)'; // 渐变
        }
    }

    updateLastUpdate() {
        const now = new Date();
        const timeString = now.toTimeString().split(' ')[0];
        document.getElementById('lastUpdate').textContent = timeString;
    }

    updateWifiDisplay() {
        // 更新SSID
        const wifiSSIDElement = document.getElementById('wifiSSID');
        if (wifiSSIDElement) {
            wifiSSIDElement.textContent = this.dualkeyState.wifiSSID || '--';
        }
        
        // 更新IP地址
        const wifiIPElement = document.getElementById('wifiIP');
        if (wifiIPElement) {
            wifiIPElement.textContent = this.dualkeyState.wifiIP || '--';
        }
        
        // 更新信号强度
        const wifiRSSIElement = document.getElementById('wifiRSSI');
        if (wifiRSSIElement) {
            if (this.dualkeyState.wifiRSSI !== 0) {
                wifiRSSIElement.textContent = `${this.dualkeyState.wifiRSSI} dBm`;
            } else {
                wifiRSSIElement.textContent = '--';
            }
        }
        
        // 更新连接状态
        const wifiStatusElement = document.getElementById('wifiStatus');
        if (wifiStatusElement) {
            if (this.dualkeyState.wifiConnected) {
                wifiStatusElement.textContent = languageManager.getText('connected');
                wifiStatusElement.className = 'value connected';
            } else {
                wifiStatusElement.textContent = languageManager.getText('notConnected');
                wifiStatusElement.className = 'value';
            }
        }
    }

    startHeartbeat() {
        // 清除之前的心跳检测
        this.stopHeartbeat();
        
        // 启动心跳检测
        this.heartbeatInterval = setInterval(() => {
            // 发送心跳包
            this.sendMessage({
                type: 'heartbeat'
            });
        }, 3000); // 每3秒发送一次心跳
        
        console.log('心跳检测已启动');
    }

    stopHeartbeat() {
        if (this.heartbeatInterval) {
            clearInterval(this.heartbeatInterval);
            this.heartbeatInterval = null;
            console.log('心跳检测已停止');
        }
    }

    startHeartbeatCheck() {
        // 启动心跳状态检查
        setInterval(() => {
            this.checkConnectionStatus();
        }, this.heartbeatCheckInterval);
    }

    checkConnectionStatus() {
        const currentTime = Date.now();
        const timeSinceLastData = currentTime - this.lastDataTime;
        
        // 如果超过离线超时时间，显示离线状态
        if (timeSinceLastData > this.offlineTimeout) {
            if (this.websocket && this.websocket.readyState === WebSocket.OPEN) {
                console.log(`心跳检查: 超过${this.offlineTimeout}ms没有收到数据，显示离线状态 (${timeSinceLastData}ms)`);
                this.updateConnectionStatus(false);
            }
        } else {
            // 如果在超时时间内有数据，且WebSocket连接正常，显示在线状态
            if (this.websocket && this.websocket.readyState === WebSocket.OPEN) {
                // 只有在状态不是在线时才更新，避免重复日志
                const statusDot = document.querySelector('.status-dot');
                if (!statusDot.classList.contains('online')) {
                    console.log(`心跳检查: 收到数据，显示在线状态 (${timeSinceLastData}ms)`);
                    this.updateConnectionStatus(true);
                }
            }
        }
    }

    // 公共方法：手动检查连接状态
    forceCheckConnection() {
        // console.log('手动检查连接状态...');
        this.checkConnectionStatus();
    }
    
    // 公共方法：设置离线超时时间
    setOfflineTimeout(timeout) {
        this.offlineTimeout = timeout;
        // console.log(`离线超时时间已设置为: ${timeout}ms`);
    }
    
    // 公共方法：获取当前连接状态信息
    getConnectionInfo() {
        const currentTime = Date.now();
        const timeSinceLastData = currentTime - this.lastDataTime;
        const wsState = this.websocket ? this.websocket.readyState : 'CLOSED';
        
        return {
            websocketState: wsState,
            timeSinceLastData: timeSinceLastData,
            offlineTimeout: this.offlineTimeout,
            isOnline: timeSinceLastData <= this.offlineTimeout && wsState === WebSocket.OPEN
        };
    }

    updateBusVisualizationFromConnectedDevices(busName, busData) {
        const visualContainer = document.getElementById(`${busName}BusDevicesVisual`);
        if (!visualContainer) {
            console.error(`未找到${busName} Bus可视化容器`);
            return;
        }
    
        // 如果没有连接设备，显示无设备提示
        if (!busData.connected_device_ids || busData.connected_device_ids.length === 0) {
            visualContainer.innerHTML = '';
            const noDevicesMsg = document.createElement('div');
            noDevicesMsg.className = 'no-devices-visual';
            noDevicesMsg.textContent = languageManager.getText('noDevices');
            visualContainer.appendChild(noDevicesMsg);
            return;
        }
    
        // 创建设备信息映射表，优先使用当前消息中的设备信息
        const deviceInfoMap = new Map();
        
        // 首先从当前消息的devices数组中获取设备信息
        if (busData.devices && Array.isArray(busData.devices)) {
            busData.devices.forEach(device => {
                if (device.id !== undefined) {
                    deviceInfoMap.set(device.id, device); // 使用设备ID作为key
                }
            });
        }

        busData.connected_device_ids.forEach(deviceId => {
            if (!deviceInfoMap.has(deviceId)) {
                const cachedDevice = this.deviceCache[busName].get(deviceId); // 直接使用设备ID获取
                if (cachedDevice) {
                    deviceInfoMap.set(deviceId, cachedDevice);
                }
            }
        });
    
        // 获取当前容器中的所有设备元素
        const existingElements = visualContainer.querySelectorAll('.chain-device-visual');
        const existingDeviceIds = new Set();
        existingElements.forEach(element => {
            const deviceId = parseInt(element.getAttribute('data-device-id'));
            if (!isNaN(deviceId)) {
                existingDeviceIds.add(deviceId);
            }
        });
    
        // 按设备ID排序连接设备
        const sortedDeviceIds = [...busData.connected_device_ids].sort((a, b) => (a || 0) - (b || 0));
        const connectedDeviceSet = new Set(busData.connected_device_ids);

        const offlineDeviceIds = [];
        existingDeviceIds.forEach(deviceId => {
            if (!connectedDeviceSet.has(deviceId)) {
                offlineDeviceIds.push(deviceId);
            }
        });

        // 找出需要添加的新设备
        const newDeviceIds = [];
        sortedDeviceIds.forEach(deviceId => {
            if (!existingDeviceIds.has(deviceId)) {
                newDeviceIds.push(deviceId);
            }
        });

        // 检查是否需要更新现有设备的类型信息
        const devicesToUpdate = [];
        existingElements.forEach(element => {
            const deviceId = parseInt(element.getAttribute('data-device-id'));
            const currentType = element.getAttribute('data-device-type');
            const deviceInfo = deviceInfoMap.get(deviceId);
            
            if (deviceInfo && deviceInfo.type && deviceInfo.type !== 'UNKNOWN' && currentType !== deviceInfo.type) {
                devicesToUpdate.push({ element, deviceInfo });
            }
        });

        // 更新现有设备的类型信息
        devicesToUpdate.forEach(({ element, deviceInfo }) => {
            this.updateDeviceVisualElement(element, deviceInfo);
        });

        // 如果没有需要添加或移除的设备，只需要重排序
        if (offlineDeviceIds.length === 0 && newDeviceIds.length === 0) {
            const currentOrder = Array.from(existingElements).map(el => parseInt(el.getAttribute('data-device-id')));
            const expectedOrder = sortedDeviceIds;
            if (JSON.stringify(currentOrder) !== JSON.stringify(expectedOrder)) {
                this.reorderDevicesWithoutAnimation(visualContainer, sortedDeviceIds);
            }
            return;
        }

        // 处理离线设备
        if (offlineDeviceIds.length > 0) {
            const sortedOfflineDeviceIds = [...offlineDeviceIds].sort((a, b) => (b || 0) - (a || 0));
            const offlineElements = [];
            sortedOfflineDeviceIds.forEach(deviceId => {
                const offlineElement = visualContainer.querySelector(`[data-device-id="${deviceId}"]`);
                if (offlineElement) {
                    offlineElements.push(offlineElement);
                }
            });

            // 播放离线动画
            offlineElements.forEach((offlineElement, index) => {
                setTimeout(() => {
                    offlineElement.style.transition = 'all 0.4s ease';
                    offlineElement.style.opacity = '0';
                    offlineElement.style.transform = 'translateY(-15px) scale(0.95)';
                    offlineElement.style.filter = 'blur(2px)';
                }, index * 100);
            });

            // 等待离线动画完成后移除离线设备并添加新设备
            setTimeout(() => {
                offlineElements.forEach(offlineElement => {
                    if (offlineElement.parentNode) {
                        offlineElement.parentNode.removeChild(offlineElement);
                    }
                });
                // 添加新设备（如果有）
                this.addNewDevices(visualContainer, newDeviceIds, deviceInfoMap, busName);
                // 重新排序所有设备
                this.reorderDevicesWithoutAnimation(visualContainer, sortedDeviceIds);
            }, 400 + Math.max(0, (offlineElements.length - 1) * 100));
        } else {
            // 只有新设备，直接添加
            this.addNewDevices(visualContainer, newDeviceIds, deviceInfoMap, busName);
            // 重新排序
            this.reorderDevicesWithoutAnimation(visualContainer, sortedDeviceIds);
        }
    }
    
    // 修改 addNewDevices 方法
    addNewDevices(visualContainer, newDeviceIds, deviceInfoMap, busName) {
        newDeviceIds.forEach(deviceId => {
            const deviceInfo = deviceInfoMap.get(deviceId);
            if (deviceInfo) {
                const deviceElement = this.createDeviceVisualElement(deviceInfo, busName);
                visualContainer.appendChild(deviceElement);
            } else {
                console.warn(`无法找到设备 ${deviceId} 的信息，跳过创建可视化元素`);
            }
        });
    }
    
    // 新增：更新现有设备可视化元素的方法
    updateDeviceVisualElement(element, deviceInfo) {
        // 更新设备类型属性
        element.setAttribute('data-device-type', deviceInfo.type);
        
        // 更新设备图片
        const deviceImage = element.querySelector('.device-image');
        if (deviceImage) {
            const newImageSrc = this.getDeviceImage(deviceInfo.type);
            if (deviceImage.src !== newImageSrc) {
                deviceImage.src = newImageSrc;
            }
        }
        
        // 更新设备类型显示名称
        const deviceTypeElement = element.querySelector('.device-type-visual');
        if (deviceTypeElement) {
            const newDisplayName = this.getDeviceDisplayName(deviceInfo.type);
            if (deviceTypeElement.textContent !== newDisplayName) {
                deviceTypeElement.textContent = newDisplayName;
            }
        }


        // 更新设备状态指示器
        const statusIndicator = element.querySelector('.device-status-indicator');
        if (statusIndicator) {
            const newStatusClass = deviceInfo.communication_flag ? '' : 'offline';
            const currentStatusClass = statusIndicator.classList.contains('offline') ? 'offline' : '';
            
            if (newStatusClass !== currentStatusClass) {
                if (newStatusClass === 'offline') {
                    statusIndicator.classList.add('offline');
                } else {
                    statusIndicator.classList.remove('offline');
                }
            }
        }
        
        console.log(`已更新设备 ${deviceInfo.id} 的可视化信息，类型: ${deviceInfo.type}, 通信状态: ${deviceInfo.communication_flag ? '正常' : '异常'}`);
    }
    
    // 修改 getDeviceInfoFromCache 方法，添加更多日志和容错处理
    getDeviceInfoFromCache(busName, deviceId) {
        if (!this.deviceCache[busName]) {
            console.log(`设备缓存不存在: ${busName}`);
            return null;
        }

        // 直接使用设备ID从缓存中获取
        const cachedDevice = this.deviceCache[busName].get(deviceId);
        if (cachedDevice && cachedDevice.type && cachedDevice.type !== 'UNKNOWN') {
            console.log(`从缓存找到设备: ${deviceId}, 类型: ${cachedDevice.type}`);
            return cachedDevice;
        } else if (cachedDevice) {
            console.log(`缓存中的设备 ${deviceId} 类型为空或UNKNOWN`);
        }

        // 如果缓存中没有找到有效信息，尝试从DOM获取
        const deviceContainer = document.getElementById(`${busName}BusDevices`);
        if (deviceContainer) {
            const deviceCard = deviceContainer.querySelector(`[data-device-id="${deviceId}"]`);
            if (deviceCard) {
                const deviceType = deviceCard.getAttribute('data-device-type');
                if (deviceType && deviceType !== 'UNKNOWN') {
                    console.log(`从DOM找到设备: ${deviceId}, 类型: ${deviceType}`);
                    return {
                        id: deviceId,
                        type: deviceType,
                        connected: true
                    };
                }
            }
        }

        console.warn(`无法获取设备 ${deviceId} 的类型信息`);
        return null; // 返回null而不是UNKNOWN设备
    }

    // 重新排序设备（不带动画）
    reorderDevicesWithoutAnimation(visualContainer, sortedDeviceIds) {
        const existingElements = Array.from(visualContainer.querySelectorAll('.chain-device-visual'));
        
        // 创建设备ID到元素的映射
        const deviceElementMap = new Map();
        existingElements.forEach(element => {
            const deviceId = element.getAttribute('data-device-id');
            if (deviceId) {
                deviceElementMap.set(String(deviceId), element);
            }
        });
        
        // 按照正确顺序重新插入元素
        const tempContainer = document.createDocumentFragment();
        sortedDeviceIds.forEach(deviceId => {
            const element = deviceElementMap.get(String(deviceId));
            if (element) {
                tempContainer.appendChild(element);
            }
        });
        
        // 只有当顺序真的需要改变时才更新DOM
        const currentOrder = Array.from(visualContainer.querySelectorAll('.chain-device-visual'))
            .map(el => el.getAttribute('data-device-id'));
        const expectedOrder = sortedDeviceIds.map(id => String(id));
        
        if (JSON.stringify(currentOrder) !== JSON.stringify(expectedOrder)) {
            // 清空容器并重新添加排序后的设备
            visualContainer.innerHTML = '';
            visualContainer.appendChild(tempContainer);
        }
    }

    // 修改创建设备可视化元素方法，添加标记避免重复动画
    createDeviceVisualElement(device, busName) {
        const deviceDiv = document.createElement('div');
        deviceDiv.className = `chain-device-visual ${busName}-device`;
        deviceDiv.setAttribute('data-device-id', device.id);
        deviceDiv.setAttribute('data-device-type', device.type);
        deviceDiv.setAttribute('data-animation-played', 'false'); // 添加动画标记
        
        // 获取设备图片
        const deviceImage = this.getDeviceImage(device.type);
        
        // 根据总线位置决定布局方向
        const isLeftBus = busName === 'left';
        const directionClass = isLeftBus ? 'left-device' : 'right-device';
        deviceDiv.className += ` ${directionClass}`;
        
        // 创建设备状态指示器
        const statusIndicator = document.createElement('div');
        statusIndicator.className = `device-status-indicator ${device.communication_flag ? '' : 'offline'}`;
        
        const html = `
            <img src="${deviceImage}" alt="${device.type}" class="device-image" 
                onerror="this.src='Chain_Blank.jpg'">
            <div class="device-info">
                <div class="device-type-visual">${this.getDeviceDisplayName(device.type)}</div>
                <div class="device-id-visual">ID: ${device.id || 'N/A'}</div>
            </div>
        `;
        deviceDiv.innerHTML = html;
        
        // 添加状态指示器
        const deviceInfo = deviceDiv.querySelector('.device-info');
        if (deviceInfo) {
            deviceInfo.appendChild(statusIndicator);
        }
        
        // 添加点击事件
        deviceDiv.addEventListener('click', () => {
            this.scrollToDeviceCard(busName, device.id);
        });
        
        // 只对新创建的设备添加入场动画
        deviceDiv.style.opacity = '0';
        deviceDiv.style.transform = isLeftBus ? 'translateX(-30px) scale(0.9)' : 'translateX(30px) scale(0.9)';
        deviceDiv.style.filter = 'blur(3px)';
        
        // 延迟播放入场动画
        setTimeout(() => {
            // 检查元素是否仍在DOM中，避免已移除的元素播放动画
            if (deviceDiv.parentNode) {
                deviceDiv.style.transition = 'all 0.4s ease';
                deviceDiv.style.opacity = '1';
                deviceDiv.style.transform = 'translateX(0) scale(1)';
                deviceDiv.style.filter = 'blur(0px)';
                deviceDiv.setAttribute('data-animation-played', 'true');
            }
        }, 50 + (parseInt(device.id) || 0) * 50);
        
        return deviceDiv;
    }

    // 获取设备图片路径
    getDeviceImage(deviceType) {
        const imageMap = {
            'KEY': 'Chain_Key.jpg',
            'JOYSTICK': 'Chain_Joystick.jpg',
            'ENCODER': 'Chain_Encoder.jpg',
            'ANGLE': 'Chain_Angle.jpg',
            'TOF': 'Chain_ToF.jpg',
            'PIR': 'Chain_PIR.jpg',
            'SWITCH': 'Chain_Switch.jpg',
            'UART': 'Chain_Uart.jpg',
            'CHAIN_BUS': 'Chain_Blank.jpg',
            'PEDAL': 'Chain_Blank.jpg',
            'MIC': 'Chain_MIC.jpg',
            'BUZZER': 'Chain_Buzzer.jpg',
            'SERVOS': 'Chain_8Servos-2.jpg',
            'MONO': 'Chain_Mono.jpg',
            'RGB': 'Chain_RGB.jpg',
            'ENV': 'Chain_ENV.jpg',
            'IMU': 'Chain_IMU.jpg',
            'DLIGHT': 'Chain_DLight.jpg',
        };

        return imageMap[deviceType] || 'Chain_Blank.jpg';
    }

    // 获取设备显示名称
    getDeviceDisplayName(deviceType) {
        const nameMap = {
            'KEY': languageManager.getText('chainKey'),
            'JOYSTICK': languageManager.getText('chainJoystick'),
            'ENCODER': languageManager.getText('chainEncoder'),
            'ANGLE': languageManager.getText('chainAngle'),
            'TOF': languageManager.getText('chainToF'),
            'PIR': languageManager.getText('chainPIR'),
            'SWITCH': languageManager.getText('chainSwitch'),
            'UART': languageManager.getText('chainUART'),
            'CHAIN_BUS': languageManager.getText('chainBusUnit'),
            'PEDAL': languageManager.getText('chainPedal'),
            'MIC': languageManager.getText('chainMIC'),
            'BUZZER': languageManager.getText('chainBuzzer'),
            'SERVOS': languageManager.getText('chainServos'),
            'MONO': languageManager.getText('chainMono'),
            'RGB': languageManager.getText('chainRGB'),
            'ENV': languageManager.getText('chainENV'),
            'IMU': languageManager.getText('chainIMU'),
            'DLIGHT': languageManager.getText('chainDLight'),
        };

        return nameMap[deviceType] || deviceType;
    }

    // 滚动到对应的设备卡片
    scrollToDeviceCard(busName, deviceId) {
        const deviceContainer = document.getElementById(`${busName}BusDevices`);
        if (!deviceContainer) return;
    
        // 直接使用设备ID查找设备卡片
        const deviceCard = deviceContainer.querySelector(`[data-device-id="${deviceId}"]`);
        if (deviceCard) {
            // 高亮设备卡片
            deviceCard.scrollIntoView({ behavior: 'smooth', block: 'center' });
            // 添加高亮效果
            deviceCard.style.boxShadow = '0 0 20px rgba(0, 100, 194, 0.6)';
            deviceCard.style.transform = 'scale(1.02)';
            setTimeout(() => {
                deviceCard.style.boxShadow = '';
                deviceCard.style.transform = '';
            }, 2000);
        }
    }

    cleanupDuplicateDevices(busName) {
        const container = document.getElementById(`${busName}BusDevices`);
        if (!container) return;
    
        const deviceElements = container.querySelectorAll('.device-item');
        const seenDeviceIds = new Set();
        const duplicateElements = [];
    
        deviceElements.forEach(element => {
            const deviceId = element.getAttribute('data-device-id');
            if (seenDeviceIds.has(deviceId)) {
                // 发现重复设备，标记为删除
                duplicateElements.push(element);
                console.warn(`发现重复设备ID: ${deviceId}，将被移除`);
            } else {
                seenDeviceIds.add(deviceId);
            }
        });
    
        // 移除重复的设备元素
        duplicateElements.forEach(element => {
            element.remove();
        });
    
        if (duplicateElements.length > 0) {
            console.log(`${busName} Bus清理了 ${duplicateElements.length} 个重复设备`);
        }
    }

    validateDeviceCache(busName) {
        const container = document.getElementById(`${busName}BusDevices`);
        if (!container) return;
    
        const domDeviceIds = new Set();
        const deviceElements = container.querySelectorAll('.device-item');
        
        deviceElements.forEach(element => {
            const deviceId = parseInt(element.getAttribute('data-device-id'));
            if (!isNaN(deviceId)) {
                domDeviceIds.add(deviceId);
            }
        });
    
        const cacheDeviceIds = new Set(this.deviceCache[busName].keys());
    
        // 检查DOM中有但缓存中没有的设备
        domDeviceIds.forEach(deviceId => {
            if (!cacheDeviceIds.has(deviceId)) {
                console.warn(`DOM中存在但缓存中缺失的设备: ${deviceId}`);
            }
        });
    
        // 检查缓存中有但DOM中没有的设备
        cacheDeviceIds.forEach(deviceId => {
            if (!domDeviceIds.has(deviceId)) {
                console.warn(`缓存中存在但DOM中缺失的设备: ${deviceId}`);
                // 可选：清理缓存中的孤立设备
                // this.deviceCache[busName].delete(deviceId);
            }
        });
    }
}

// 页面加载完成后初始化
document.addEventListener('DOMContentLoaded', () => {
    const controller = new DualKeyController();
    
    // 将controller设为全局变量，方便调试
    window.dualkeyController = controller;
    // console.log('DualKeyController 已设为全局变量: window.dualkeyController');
    
    // 添加键盘快捷键提示
    // const helpText = document.createElement('div');
    // helpText.style.cssText = `
    //     position: fixed;
    //     bottom: 20px;
    //     right: 20px;
    //     background: rgba(0, 0, 0, 0.8);
    //     color: white;
    //     padding: 15px;
    //     border-radius: 10px;
    //     font-size: 14px;
    //     z-index: 1000;
    //     min-width: 200px;
    // `;
    // helpText.innerHTML = `
    //     <strong>键盘快捷键:</strong><br>
    //     PGUP - 左键<br>
    //     PGDN - 右键<br><br>
    //     <strong>WebSocket状态:</strong><br>
    //     <span id="ws-status">连接中...</span><br><br>
    //     <strong>最后更新:</strong><br>
    //     <span id="lastUpdate">--:--:--</span>
    // `;
    // controller.forceCheckConnection() 手动检查连接状态
    // controller.getConnectionInfo() 获取连接状态信息
    // document.body.appendChild(helpText);
});

// 添加一些视觉效果
document.addEventListener('DOMContentLoaded', () => {
    // 为卡片添加进入动画
    const cards = document.querySelectorAll('.card');
    cards.forEach((card, index) => {
        card.style.opacity = '0';
        card.style.transform = 'translateY(20px)';
        
        setTimeout(() => {
            card.style.transition = 'all 0.6s ease';
            card.style.opacity = '1';
            card.style.transform = 'translateY(0)';
        }, index * 100);
    });
    
    // 为按钮添加点击效果
    const buttons = document.querySelectorAll('.btn');
    buttons.forEach(button => {
        button.addEventListener('click', function() {
            this.style.transform = 'scale(0.95)';
            setTimeout(() => {
                this.style.transform = 'scale(1)';
            }, 150);
        });
    });
});
