#version 330 core

struct Material{
    // 材质
    sampler2D colorTexture;  // 颜色贴图
    int isColorTexVal;
    sampler2D specTexture; // 镜面反射贴图
    int isSpecTexVal;
    sampler2D dumpTexture;  // 凹凸贴图
    int isDumpTexVal;
    float ns_value;  // 锐利值(shininess)
    float ni_value;  // 不知道干啥的
    float d_value;   // 不知道干啥的
    vec3 ka_value;  // 环境光系数
    vec3 kd_value;  // 漫反射系数
    vec3 ks_value;  // 镜面反射系数
};

struct PointLight{
    // 最基础的点光源
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float attenuation_constant;  //衰减常数项
    float attenuation_linear;    //衰减线性项
    float attenuation_quadratic; //衰减二次项
};

struct DirectionalLight{
    //平行光(定向光)
    //无需光源位置 (光源无限远)
    vec3 direction; //光的方向 三维向量
    vec3 ambient; //环境光颜色
    vec3 diffuse; //漫反射颜色
    vec3 specular; //镜面反射颜色;
};

struct FlashLight{
    int flashSwitch; // 开关
    vec3 ambient;  //环境光颜色
    vec3 diffuse;  //漫反射颜色
    vec3 specular; //镜面反射颜色
    //聚光(手电筒)
    vec3 position; //手电筒位置
    vec3 direction; //手电筒方向
    float cutOff; //手电筒半切角的余弦值
    float outerCutOff; //外圆锥余弦值
    float attenuation_constant;  //衰减常数项
    float attenuation_linear;    //衰减线性项
    float attenuation_quadratic; //衰减二次项
};

// uniform 变量
uniform vec3 viewPos; // 摄像机朝向单位向量
uniform Material material; // 物体的材质信息
uniform FlashLight flashLight; // 聚光光源A
uniform PointLight pointLight; // 点光源B


// 顶点着色器传来的数据
in vec2 texCoord;  // uv
in vec3 outNormal; // 法向量
in vec3 fragPos;  // 顶点位置

// 最后的输出
out vec4 color;

vec3 calFlashLight(FlashLight f_light);
vec3 calPointLight(PointLight p_light);

void main(){
    vec3 result = calPointLight(pointLight) + calFlashLight(flashLight);
    color = vec4(result, 1.0);
}

vec3 calPointLight(PointLight p_light){
    vec3 lightDir = normalize(p_light.position - fragPos);
    float diff = max(dot(normalize(outNormal), lightDir), 0.0);
    // 镜面光着色
    vec3 reflectDir = reflect(-lightDir, normalize(outNormal));
    vec3 viewVec = normalize(viewPos - fragPos);
    float spec = pow(max(dot(reflectDir, viewVec), 0.0), material.ns_value);
    // 衰减
    float distance    = length(p_light.position - fragPos);
    float attenuation = 1.0 / (p_light.attenuation_constant + p_light.attenuation_linear * distance +
                 p_light.attenuation_quadratic * (distance * distance));
    // 合并结果
    vec3 ambient  = p_light.ambient  * material.ka_value * vec3(texture(material.colorTexture, texCoord));
    vec3 diffuse  = p_light.diffuse  * material.kd_value * diff * vec3(texture(material.colorTexture, texCoord)) ;
    vec3 specular = p_light.specular * material.ks_value * spec * vec3(texture(material.specTexture, texCoord));
    return (ambient + diffuse + specular) * attenuation;
}

vec3 calFlashLight(FlashLight f_light){
    if(f_light.flashSwitch == 0)
        return vec3(0.0, 0.0, 0.0);
    // ambient
    vec3 ambient = f_light.ambient * material.ka_value * vec3(texture(material.colorTexture, vec2(texCoord)));

    // diffuse
    vec3 norm = normalize(outNormal);
    vec3 lightVec = normalize(f_light.position - fragPos);
    float diffuseCoe = max(dot(norm, lightVec), 0.0);
    vec3 diff = f_light.diffuse * material.kd_value * (diffuseCoe * vec3(texture(material.colorTexture, texCoord)));

    // spec
    vec3 reflectVec = reflect(-lightVec, norm);
    vec3 viewVec = normalize(viewPos - fragPos);
    float specularCoe = pow(max(dot(reflectVec, viewVec), 0.0), material.ns_value);
    vec3 spec = f_light.specular * material.ks_value * (specularCoe * vec3(texture(material.specTexture, texCoord)));

    //cutOff
    float theta = dot(lightVec, normalize(-f_light.direction));
    float epsilon = f_light.cutOff - f_light.outerCutOff;
    float cut = clamp((theta - f_light.outerCutOff) / epsilon, 0.0, 1.0);

    float distance = length(f_light.position - fragPos);
    float attenuation = 1.0f / (f_light.attenuation_constant + f_light.attenuation_linear * distance + f_light.attenuation_quadratic * (distance * distance));

    vec3 result = ambient + diff * cut * attenuation + spec * cut * attenuation;
    return result;
}



