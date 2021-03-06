//
//  XMUtilities.m
//  XMBindingLibrarySample
//
//  Created by Anuj Bhatia on 1/18/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import "XMUtilities.h"

@implementation XMUtilities

-(id) init
{
    if(self = [super init]) {
        // do initialization here after super init nil check!
    }
    
    return self;
}

-(void) dealloc
{
    // this is an ARC project so we don't have to dealloc
    // we dont even have to call [super dealloc];
    // old habits die hard, Yippee-ki-yay!
}

// This is an example of a class method. It will echo the message you give it.
// Obj-C class methods are like C# static methods, but different.

+(NSString *) echo:(NSString *)message
{
    if([message length] == 0) {
        return [NSString stringWithFormat:@"Dude %@, you didnt give me a message!", @"bro"];
    }
    
    return [NSString stringWithFormat:@"%@", message];
}

// This is an example of an instance method.

-(NSString *) hello:(NSString *)name
{
    if([name length] == 0) {
        return [NSString stringWithFormat:@"Dude %@, you didnt give me a name!", @"bro"];
    }
    
    return [NSString stringWithFormat:@"*Waves* Hello %@! Welcome to the Xamarin binding sample!", name];
}

-(NSInteger) add:(NSInteger)operandUn and:(NSInteger) operandDeux
{
    return operandUn + operandDeux;
}

-(NSInteger) multiply:(NSInteger)operandUn and:(NSInteger)operandDeux
{
    return operandUn * operandDeux;
}

// This is an example of how to set a block function for later use.
-(void) setCallback:(XMUtilityCallback) callback
{
	_callback = [callback copy];
}

// This is an example of how to invoke a block function.
-(void) invokeCallback:(NSString *) message
{
	_callback (message);
}

@end


void XamarinInit(int w, int h, int windowWidth, int windowHeight)
{
    extern void XamarinInit_(int w, int h, int windowWidth, int windowHeight);
    XamarinInit_(w, h, windowWidth, windowHeight);
}

void XamarinDeinit()
{
    extern void XamarinDeinit_();
    XamarinDeinit_();
}

void XamarinRender()
{
    void XamarinRender_();
    XamarinRender_();
}


void XamarinTouchesBegan(uint64_t touchId, int x, int y)
{
    void XamarinTouchesBegan_(uint64_t touchId, int x, int y);
    XamarinTouchesBegan_(touchId, x, y);
}

void XamarinTouchesMoved(uint64_t touchId, int x, int y)
{
    void XamarinTouchesMoved_(uint64_t touchId, int x, int y);
    XamarinTouchesMoved_(touchId, x, y);
}

void XamarinTouchesEnded(uint64_t touchId, int x, int y)
{
    void XamarinTouchesEnded_(uint64_t touchId, int x, int y);
    XamarinTouchesEnded_(touchId, x, y);
}

void XamarinTouchesCancelled(uint64_t touchId)
{
    void XamarinTouchesCancelled_(uint64_t touchId);
    XamarinTouchesCancelled_(touchId);
}

