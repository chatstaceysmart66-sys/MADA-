# تسليم المشروع إلى وكيل برمجة AI

ابدأ من جذر مستودع المشروع. اقرأ `AGENTS.md` ثم ملف المواصفات كاملاً `Docs/MASTER_SPEC_AR_v0.3.txt`، ثم حالة التنفيذ وتدقيق الاعتماديات والمشكلات المعروفة.

قبل التعديل:
1. افحص `git status` والفرع والـcommit والتغييرات الموجودة.
2. افحص Unreal Engine 5.8 وcompiler وAndroid SDK/NDK/JDK وADB وأجهزة الاختبار المتاحة.
3. شغّل:

```bash
python3 Scripts/validate_project.py
python3 Scripts/audit_files.py
python3 -m unittest discover -s Tests -v
bash Scripts/run_host_tests.sh
```

4. افتح المشروع في UE 5.8 وشغّل UHT ثم ابنِ Editor/Game فعلياً. أصلح أخطاء UHT وCompile وLink دون حذف الوظائف أو إضعاف الاختبارات.
5. ثبّت اختبار Vertical Slice: القائمة، المشي، دخول/خروج السيارة، القيادة، الحفظ والتحميل، ثم انتقل إلى Chaos Vehicles والأصول المرخصة بعد ثبات الأساس.
6. لا تدّعِ APK أو FPS أو Offline أو لمس أو حرارة ناجحة قبل سجل بناء وتثبيت واختبار على جهاز فعلي.
7. لا تستخدم محتوى مستخرجاً من GTA/Forza/RDR. سجل كل أصل ورخصته قبل إدخاله.
8. حدّث ملفات `Docs/BUILD_REPORT.md`, `Docs/TEST_REPORT.md`, `Docs/FILE_AND_DEPENDENCY_AUDIT.md`, `Docs/ANDROID_DEVICE_TEST_MATRIX.csv`, `Docs/ASSET_LICENSE_MANIFEST.csv`, `Docs/KNOWN_ISSUES.md`, `Docs/DELIVERY_MANIFEST.txt`, و`Docs/WORKLOG.md` بعد كل دورة.

الحالة عند التسليم الحالي: مشروع مصدر مدمج مع اختبارات المضيف، وليس مشروع Unreal مبنياً أو APK.
