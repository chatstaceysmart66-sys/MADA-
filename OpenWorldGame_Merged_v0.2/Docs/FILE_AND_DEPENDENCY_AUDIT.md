# تدقيق الملفات والاعتماديات — OpenWorldGame Merged v0.2

## قاعدة الدمج

تم اعتماد `OWG_Android_Starter_v0.1` كأساس لأنه يحتوي على أنظمة C++ للعبة. أضيفت إليه ملفات Core والاختبارات وسكربتات التدقيق وWorkflow من `OpenWorldGame_scaffold`، مع تجنب استبدال ملفات اللعب الموجودة. لم يتم نقل مجلد `.git` الموجود داخل الأرشيف الثاني؛ كان بيانات مستودع فارغ/غير مكتمل ولا يلزم داخل حزمة المصدر.

## الوحدات والمسارات

| المسار | الاستخدام | حالة الملف |
|---|---|---|
| `OpenWorldGame.uproject` | تعريف UE 5.8 ووحدة Runtime | موجود، JSON صالح في فحص Python |
| `Source/OpenWorldGame/OpenWorldGame.cpp` | تسجيل وحدة Unreal | المسار القياسي موجود؛ لم يُبنَ بالمحرك |
| `Source/OpenWorldGame/OpenWorldGame.Build.cs` | اعتمادات الوحدة | موجود |
| `Source/OpenWorldGame.Target.cs` | هدف Game | موجود |
| `Source/OpenWorldGameEditor.Target.cs` | هدف Editor | موجود |
| `Source/OpenWorldGame/Public` + `Private` | الشخصية والمركبة وGameMode وGameInstance وUMG | موجودة |
| `Source/OpenWorldGame/Core` | اعتماد المطور وتحويل السرعة وقياس FPS وقرار دخول/خروج | موجودة ومختبرة على المضيف؛ بعضها مربوط بمسار runtime |
| `Source/OpenWorldGame/SaveSystem/OWSaveData.h` | نموذج حفظ مستقل مع CRC وترحيل v1→v2 | موجود ومختبر؛ لا يستخدمه runtime حالياً |
| `Config/DefaultEngine.ini` | GameMode/GameInstance وإعداد Android arm64 | موجود |
| `Config/DefaultGame.ini` | هوية المشروع وإعداد لاعب واحد | موجود |
| `Config/DefaultInput.ini` | إدخال لوحة المفاتيح/Virtual Joystick | موجود |
| `Config/DefaultScalability.ini` | جودة أولية | موجود |
| `Scripts/validate_project.py` | تدقيق ثابت للملفات الأساسية وربط الإدخال | موجود |
| `Scripts/audit_files.py` | تدقيق المسارات والإعدادات والـincludes المحلية | موجود |
| `Scripts/run_host_tests.sh` | بناء وتشغيل اختبارات المنطق المستقلة | موجود |
| `Tests/test_project_integrity.py` | اختبارات Python للبنية والربط | موجود |
| `Tests/HostTests/test_main.cpp` | اختبارات C++ مستقلة عن Unreal | موجود |
| `.github/workflows/host-tests.yml` | تشغيل فحوص المضيف في GitHub Actions | موجود؛ لم يتم تشغيل CI عن بعد |

## إعداد Android

- Package: `com.abdullahlazim.openworldgame`.
- ABI المستهدف: ARM64 فقط في إعدادات المشروع.
- Minimum install SDK: 26؛ SDK platforms 34 و35 وBuild Tools 35.0.1 مثبتة الآن داخل `/workspaces/.toolchains/android-sdk` حسب إعداد المحرك المرجعي؛ راجع إعدادات UE عند التغليف.
- Landscape، Vulkan مفعّل كمسار إعداد.
- `ExtraPermissions=` فارغ ولا توجد صلاحية INTERNET مضافة.
- NDK r27c وPlatform Tools 37.0.1 وMicrosoft JDK 21.0.3 مثبتة خارج Git؛ جرى فحص ARM64 compiler probe. هذه ليست إثباتاً لنجاح Unreal Android packaging.

## الاعتماديات والإضافات

- Build.cs يعتمد `Core`, `CoreUObject`, `Engine`, `RHI`, `PhysicsCore`, `InputCore`, `UMG`, `Slate`, و`SlateCore`؛ أضيف `RHI` لاستعلام Frame Pacer، ولم يُتحقق من تجميعه داخل UE 5.8.
- لا توجد Plugins خارجية مفعّلة في `.uproject` الحالي.
- لم تُنسخ إعدادات تمكين `ChaosVehiclesPlugin` أو `EnhancedInput` من scaffold لأنها غير مستخدمة في نظام المصدر الحالي؛ المركبة ليست Chaos Vehicle ونظام الإدخال الحالي يستخدم InputCore. لا ينبغي تفعيل Plugin دون إدماج فعلي وبناء بالمحرك.
- ملفات `.generated.h` تُنتجها UnrealHeaderTool ولا توجد ضمن المصدر الخام.

## نتائج الفحص المحلي

راجع الملفات التالية للتفاصيل والأوامر:

- `Docs/STATIC_VALIDATION_RESULTS.txt`
- `Docs/PYTHON_TEST_RESULTS.txt`
- `Docs/logs/file_audit.log`
- `Docs/logs/host_tests.log`
- `Docs/BUILD_REPORT.md`
- `Docs/TEST_REPORT.md`

## ما لا يمكن حسمه دون Unreal/Android

UHT، ترجمة/ربط C++ عبر UnrealBuildTool، توفر Engine Basic Shapes بعد Android Cook، صلاحية مراجع الأصول، فتح القائمة وتشغيل الأزرار، الفيزياء وPossess في Runtime، التغليف والتوقيع، تثبيت APK، Offline mode على هاتف، اللمس، FPS/CPU/GPU/RAM/الحرارة.

## متابعة التدقيق — 2026-11-11

- الأساس: `ced4a05` على `main`، مجلد العمل `OpenWorldGame_Merged_v0.2`.
- `python3 Scripts/validate_project.py`: PASS؛ `python3 Scripts/audit_files.py`: PASS 27/27؛ Python integrity: PASS 16/16؛ C++ host: PASS 42/42.
- فحص ملفات المحتوى لا يجد `.umap` أو `.uasset` أو `.uplugin` أو `MADA_Final_Android_ARM64.apk`. العالم الحالي Greybox منشأ من C++ وليس خريطة مدينة/ريف نهائية.
- Unreal/Android: BLOCKED لغياب UnrealEditor/UBT وAndroid SDK/NDK وADB والجهاز. إضافة RHI وتدفق تأكيد الحفظ لم يُختبرا بالمحرك.

## تدقيق Codespaces — 2026-11-11

- Android SDK/NDK/JDK الآن مثبتة خارج المستودع في `/workspaces/.toolchains`; التفاصيل في `Docs/BUILD_ENVIRONMENT_REPORT.txt`.
- `adb` موجود لكن لا يوجد جهاز في `adb devices -l`. Android ARM64 NDK probe نجح، دون إنشاء APK.
- Unreal Engine/UBT/RunUAT ما زالت غير موجودة؛ `EpicGames/UnrealEngine` غير متاح لحساب GitHub الحالي. لا توجد ملفات `.umap` أو `.uasset` أو `.uplugin` أو APK بالمشروع.
- لم يتغير الحكم: UHT/UBT/cook/package/device tests BLOCKED/NOT RUN.
