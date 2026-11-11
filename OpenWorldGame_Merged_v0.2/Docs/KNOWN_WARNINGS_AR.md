# التحذيرات والموانع — MADA

## موانع الإصدار

- **BLOCKED:** لا يوجد Unreal Engine 5.8 أو UnrealHeaderTool/UnrealBuildTool في الحاوية؛ توافق UHT وCompile/Link غير مثبت.
- **BLOCKED:** أدوات Android مثبتة في `/workspaces/.toolchains`، لكن Unreal Engine/UBT غير متاحين فلا يمكن بناء APK. ADB يعمل ولا يوجد جهاز فعلي لاختبارات التثبيت/Offline/اللمس/الأداء.
- **NOT IMPLEMENTED:** العالم الحالي Greybox برمجي؛ لا توجد خرائط `.umap` أو مدينة/ريف مترابطان أو World Partition/HLOD/Streaming.
- **NOT IMPLEMENTED:** المركبة Rigid-Body prototype وليست Chaos Vehicle؛ لا تعليق أو إطارات/جر واقعي أو صوت محرك.
- **NOT IMPLEMENTED:** لا أصول نهائية مرخصة لشخصية أو مركبة أو بيئة أو صوت، ولا Traffic/NPC أو طقس ودورة ليل/نهار.

## تغييرات مصدرية لم تُختبر بالمحرك

- **NOT RUN:** قائمة Pause، استقبال Escape أثناء التوقف، انتقال New Game عبر `OpenLevel`، حذف SaveGame، وبدء اللعب بعد الانتقال.
- **NOT RUN:** لوحة تأكيد استبدال الحفظ الجديدة لم تُشغّل داخل Unreal؛ اختبارات Python تتحقق من الوجود والربط الساكن فقط.
- **NOT RUN:** حفظ/تحميل `USaveGame` بعد إعادة تشغيل التطبيق، بما في ذلك وضع المركبة.
- **NOT RUN:** ظهور العربية واتجاه RTL ومناطق الأمان وتخطيط الواجهة على الهاتف.
- **NOT RUN:** تجميع تكامل FPS مع `FPlatformRHIFramePacer` أو فحص خيارات معدل تحديث شاشة Android؛ الاختبار الساكن لا يثبت توافق Unreal أو الجهاز.
- **BLOCKED:** تثبيت Unreal غير ملائم لهذا المضيف: 19 GiB فقط كانت متاحة قبل تنزيل الأدوات؛ الجهاز 2 vCPU و7.8 GiB RAM وUbuntu 24.04 ومحوله الرسومي Hyper-V، بينما توصي Epic بـUbuntu 22.04 و32 GiB RAM وGPU بذاكرة 8 GiB. وصول GitHub الحالي لا يسمح بقراءة `EpicGames/UnrealEngine`.

## ما يمكن إثباته محلياً

- Python integrity tests: 17/17 في متابعة إعداد Codespaces بتاريخ 2026-11-11.
- C++ host tests: 42/42؛ ASan/UBSan: 42/42. هذه اختبارات منطق مستقل عن Unreal.
- Static file/dependency audit: 27/27 في آخر تشغيل موثق.
- بصمات manifest يجب إعادة توليدها بعد تثبيت مرحلة التنفيذ، لأن ملفات السجلات والوثائق تتغير مع الاختبارات.
