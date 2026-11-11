# سجل التنفيذ — MADA

## 2026-11-11 — استئناف التنفيذ

- **نقطة البداية:** commit `d8b1040` على `main`؛ ملف ZIP محذوف حسب طلب المالك. لم أُنشئ commit أو أدفع التغييرات الحالية.
- **الهوية:** MADA | مَدَى وBEYOND THE HORIZON والاعتماد «تطوير: عبدالله لازم» موجودة في إعدادات المشروع والواجهة.
- **New Game / Continue:** أضيف تدفق New Game يحذف slot الحفظ ويعيد تحميل المستوى الحالي، ثم يبدأ اللعب بعد انتقال المستوى. Continue يحمل الحفظ، أو ينشئ جلسة جديدة إذا تعذر التحميل.
- **Pause:** أضيف فتح/إغلاق من Escape ومن زر اللمس، مع خلفية تعتيم وزري استئناف وحفظ. يضبط وضع الإدخال والمؤشر ويطلب Pause للعالم.
- **الإعدادات:** اختيار Frame Cap يطبق على `UGameUserSettings` ويحفظ محلياً؛ لا يوجد فحص Refresh Rate أو دعم Frame Pacing مخصص حتى الآن.
- **الاختبارات:** Python integrity 14/14؛ C++ host 42/42؛ ASan/UBSan 42/42؛ static file audit 27/27. اختبارات Python وC++ تختبر الربط والمنطق المستقل، ولا تثبت سلوك Unreal Runtime.
- **البيئة:** Unreal Engine 5.8 وUnrealBuildTool وAndroid SDK/NDK وADB وجهاز Android غير متاحة.

## الخطوة التالية الدقيقة

على بيئة تثبيت Unreal Engine 5.8: شغّل UnrealHeaderTool/UnrealBuildTool على `OpenWorldGame.uproject`، أصلح أخطاء البناء أولاً، ثم اختبر في PIE بالترتيب: شاشة MADA، New Game وإعادة تحميل المستوى، Continue بعد حفظ فعلي، Pause/Resume عبر Escape واللمس، ثم حفظ/تحميل المركبة. لا تنتقل إلى APK قبل نجاح هذه الاختبارات. بعد ذلك جهّز Android SDK/NDK المطابق وابنِ arm64، ثم ثبّت الحزمة على هاتف حقيقي.
