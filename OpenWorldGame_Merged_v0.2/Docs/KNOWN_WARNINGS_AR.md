# التحذيرات والموانع — MADA

## موانع الإصدار

- **BLOCKED:** لا يوجد Unreal Engine 5.8 أو UnrealHeaderTool/UnrealBuildTool في الحاوية؛ توافق UHT وCompile/Link غير مثبت.
- **BLOCKED:** لا يوجد Android SDK/NDK أو ADB أو جهاز Android؛ لا يمكن بناء APK أو تثبيته أو اختبار Offline/اللمس/الأداء.
- **NOT IMPLEMENTED:** العالم الحالي Greybox برمجي؛ لا توجد خرائط `.umap` أو مدينة/ريف مترابطان أو World Partition/HLOD/Streaming.
- **NOT IMPLEMENTED:** المركبة Rigid-Body prototype وليست Chaos Vehicle؛ لا تعليق أو إطارات/جر واقعي أو صوت محرك.
- **NOT IMPLEMENTED:** لا أصول نهائية مرخصة لشخصية أو مركبة أو بيئة أو صوت، ولا Traffic/NPC أو طقس ودورة ليل/نهار.

## تغييرات مصدرية لم تُختبر بالمحرك

- **NOT RUN:** قائمة Pause، استقبال Escape أثناء التوقف، انتقال New Game عبر `OpenLevel`، حذف SaveGame، وبدء اللعب بعد الانتقال.
- **NOT RUN:** حفظ/تحميل `USaveGame` بعد إعادة تشغيل التطبيق، بما في ذلك وضع المركبة.
- **NOT RUN:** ظهور العربية واتجاه RTL ومناطق الأمان وتخطيط الواجهة على الهاتف.
- **NOT RUN:** تجميع تكامل FPS مع `FPlatformRHIFramePacer` أو فحص خيارات معدل تحديث شاشة Android؛ الاختبار الساكن لا يثبت توافق Unreal أو الجهاز.

## ما يمكن إثباته محلياً

- Python integrity tests: 15/15 في متابعة 2026-11-11.
- C++ host tests: 42/42؛ ASan/UBSan: 42/42. هذه اختبارات منطق مستقل عن Unreal.
- Static file/dependency audit: 27/27 في آخر تشغيل موثق.
- بصمات manifest يجب إعادة توليدها بعد تثبيت مرحلة التنفيذ، لأن ملفات السجلات والوثائق تتغير مع الاختبارات.
