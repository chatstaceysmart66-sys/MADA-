# مراجع API رسمية تم الرجوع إليها أثناء كتابة النواة

هذه المراجع تثبت وجود واجهات المحرك التي استُخدمت في المصدر؛ لا تثبت أن المشروع بُني أو اشتغل في بيئة المستخدم.

- UGameUserSettings / SetFrameRateLimit / ApplySettings: https://dev.epicgames.com/documentation/unreal-engine/API/Runtime/Engine/UGameUserSettings
- UPrimitiveComponent / AddForce / AddTorqueInRadians / SetMassOverrideInKg: https://dev.epicgames.com/documentation/en-us/unreal-engine/API/Runtime/Engine/UPrimitiveComponent
- UCollisionProfile / PhysicsActor profile name: https://dev.epicgames.com/documentation/en-us/unreal-engine/API/Runtime/Engine/UCollisionProfile
- UUserWidget::GetOwningPlayerPawn: https://dev.epicgames.com/documentation/unreal-engine/API/Runtime/UMG/UUserWidget/GetOwningPlayerPawn
- UCanvasPanelSlot: https://dev.epicgames.com/documentation/unreal-engine/API/Runtime/UMG/UCanvasPanelSlot
- FCollisionShape: https://dev.epicgames.com/documentation/unreal-engine/API/Runtime/PhysicsCore/FCollisionShape
- UGameplayStatics::GetAllActorsOfClass: https://dev.epicgames.com/documentation/en-us/unreal-engine/API/Runtime/Engine/UGameplayStatics/GetAllActorsOfClass
- APlayerController::SetPause: https://dev.epicgames.com/documentation/unreal-engine/API/Runtime/Engine/APlayerController/SetPause

كل API يجب أن يُراجع ويُبنى من جديد ضمن Unreal Engine 5.8 في جهاز التطوير. إذا أظهر UHT أو compiler اختلافاً في توقيع أو تضمين، أصلح وفق headers المثبتة في نسخة المحرك الفعلية وسجّل ذلك في WORKLOG.
