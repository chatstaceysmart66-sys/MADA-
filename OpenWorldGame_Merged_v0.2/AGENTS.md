# تعليمات وكلاء البرمجة

اقرأ `Docs/MASTER_SPEC_AR_v0.3.txt` بالكامل قبل التغيير. راجع كذلك `Docs/IMPLEMENTATION_STATUS_AR.md` و`Docs/FILE_AND_DEPENDENCY_AUDIT.md` و`Docs/KNOWN_ISSUES_AR.md`.

## قواعد غير قابلة للتفاوض

- احفظ الملفات والعمل الموجود؛ لا تستخدم `git reset --hard` أو `git clean -fdx` أو Force Push أو حذفاً جماعياً.
- لا تدّعِ Compile أو Gameplay أو APK أو Offline أو FPS أو Thermal PASS دون تنفيذ الاختبار وحفظ الدليل.
- لا تخترع أصولاً أو صوراً أو خرائط أو أصواتاً أو تراخيص.
- أبقِ اللعبة فردية وتعمل دون إنترنت؛ لا تضف حسابات أو API إلزامية.
- Android arm64-v8a هو هدف التسليم الأساسي؛ Windows بيئة تطوير وبناء.
- المركبة الحالية Rigid-Body prototype وليست Chaos Vehicle.
- لا تصف Greybox على أنها مدينة أو عالم نهائي.
- لا تفعّل Plugins لمجرد وجودها في scaffold؛ يجب دمجها واختبارها بالمحرك.
- حدّث الوثائق والاختبارات مع كل تغيير.

## سير العمل الإلزامي

1. افحص حالة المستودع والـtoolchain.
2. نفّذ `python3 Scripts/validate_project.py` و`python3 Scripts/audit_files.py` واختبارات Python وC++.
3. نفّذ ميزة واحدة في كل مرة مع شروط قبول واضحة.
4. ابنِ باستخدام Unreal Engine 5.8 عند توفره.
5. شغّل Unreal Automation/Functional Tests المناسبة.
6. ابنِ Android arm64 فقط بعد إعداد SDK/NDK/JDK الموثق.
7. ثبّت الحزمة على هاتف فعلي واختبر Offline واللمس والحفظ والأداء.
8. عند غياب أداة أو جهاز، استخدم `BLOCKED` أو `NOT RUN` مع السبب؛ لا تستبدلها بـPASS.
