//  Sphere.h
//  Created by Gauri on 8/9/21.
//
#import <Foundation/Foundation.h>
#include<math.h>

@interface MySphere:NSObject
-(void) getSphereVertexData:(float[])spherePositionCoords :(float[])sphereNormalCoords :(float[])sphereTexCoords :(unsigned short[])sphereElements;

-(int) getNumberOfSphereVertice;
-(int) getNumberOfSphereElements;
-(void) processSphereData;
-(void) addTriangle:(float[][3])single_vertex :(float[][3])single_normal     :(float[][2])single_texture;
-(void) normalizeVector:(float[])v;
-(bool) isFoundIdentical:(float)val1  vertex:(float)val2 differ:(float)diff;

@end

