/*
Navicat MySQL Data Transfer

Source Server         : localhost_3306
Source Server Version : 50553
Source Host           : localhost:3306
Source Database       : postdb

Target Server Type    : MYSQL
Target Server Version : 50553
File Encoding         : 65001

Date: 2017-06-10 09:17:09
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for academic_education
-- ----------------------------
DROP TABLE IF EXISTS `academic_education`;
CREATE TABLE `academic_education` (
  `number` int(11) NOT NULL AUTO_INCREMENT,
  `code` varchar(20) NOT NULL,
  `begin` date DEFAULT NULL,
  `end` date DEFAULT NULL,
  `university` varchar(50) DEFAULT NULL,
  `department` varchar(50) DEFAULT NULL,
  `major` varchar(20) DEFAULT NULL,
  `diploma` varchar(20) DEFAULT NULL,
  `degree` varchar(20) DEFAULT NULL,
  PRIMARY KEY (`number`),
  KEY `code` (`code`),
  CONSTRAINT `academic_education_ibfk_1` FOREIGN KEY (`code`) REFERENCES `staff` (`code`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of academic_education
-- ----------------------------
INSERT INTO `academic_education` VALUES ('1', '3261998002', '2009-08-23', '2012-06-04', '华南理工大学', '计算机系', '软件工程', '研究生', '硕士');

-- ----------------------------
-- Table structure for administrative_post
-- ----------------------------
DROP TABLE IF EXISTS `administrative_post`;
CREATE TABLE `administrative_post` (
  `ID` int(11) NOT NULL AUTO_INCREMENT,
  `rank` varchar(50) NOT NULL,
  `description` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of administrative_post
-- ----------------------------
INSERT INTO `administrative_post` VALUES ('1', '副处长', '行政部门一般有2-3名副处长。');
INSERT INTO `administrative_post` VALUES ('2', '处长', '');

-- ----------------------------
-- Table structure for appendant
-- ----------------------------
DROP TABLE IF EXISTS `appendant`;
CREATE TABLE `appendant` (
  `code` varchar(20) NOT NULL,
  `name` varchar(50) NOT NULL,
  `leader` varchar(50) NOT NULL,
  `location` varchar(100) DEFAULT NULL,
  `organization` varchar(255) DEFAULT NULL,
  `regulation` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`code`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of appendant
-- ----------------------------
INSERT INTO `appendant` VALUES ('1056151', '医学院', '袁绍', '医学院', '本单位有3个专业：临床医学、药理学、护理学。', '本单位招收本科、专科两个学历培养层次的学生，欢迎报考。');

-- ----------------------------
-- Table structure for auxiliary
-- ----------------------------
DROP TABLE IF EXISTS `auxiliary`;
CREATE TABLE `auxiliary` (
  `code` varchar(20) NOT NULL,
  `name` varchar(50) NOT NULL,
  `leader` varchar(50) NOT NULL,
  `location` varchar(100) DEFAULT NULL,
  `organization` varchar(255) DEFAULT NULL,
  `duty` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`code`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of auxiliary
-- ----------------------------
INSERT INTO `auxiliary` VALUES ('1056141', '学报编辑部', '孙权', '行政楼', '本部门有3个组室：理工组、社科组、办公室。', '服务于本校师生，也欢迎外校作者投稿。');

-- ----------------------------
-- Table structure for class_teaching
-- ----------------------------
DROP TABLE IF EXISTS `class_teaching`;
CREATE TABLE `class_teaching` (
  `number` int(11) NOT NULL AUTO_INCREMENT,
  `code` varchar(20) NOT NULL,
  `annual` varchar(20) DEFAULT NULL,
  `semester` varchar(20) DEFAULT NULL,
  `course` varchar(20) DEFAULT NULL,
  `sort` varchar(20) DEFAULT NULL,
  `level` varchar(20) DEFAULT NULL,
  `class` varchar(30) DEFAULT NULL,
  `object` varchar(20) DEFAULT NULL,
  `location` varchar(100) DEFAULT NULL,
  `hour` varchar(20) DEFAULT NULL,
  PRIMARY KEY (`number`),
  KEY `code` (`code`),
  CONSTRAINT `class_teaching_ibfk_1` FOREIGN KEY (`code`) REFERENCES `staff` (`code`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of class_teaching
-- ----------------------------
INSERT INTO `class_teaching` VALUES ('1', '3261998001', '2016-2017', '2', '数据库课程设计', '实验', '专业必修课', '15计科01', '本科生', '信工楼', '48');

-- ----------------------------
-- Table structure for contacts
-- ----------------------------
DROP TABLE IF EXISTS `contacts`;
CREATE TABLE `contacts` (
  `number` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(50) DEFAULT NULL,
  `sex` varchar(10) DEFAULT NULL,
  `fPhone` varchar(20) DEFAULT NULL,
  `oPhone` varchar(20) DEFAULT NULL,
  `mPhone` varchar(20) DEFAULT NULL,
  `short` varchar(20) DEFAULT NULL,
  `qq` varchar(20) DEFAULT NULL,
  `wechat` varchar(50) DEFAULT NULL,
  `email` varchar(50) DEFAULT NULL,
  `unit` varchar(50) DEFAULT NULL,
  `remark` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`number`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of contacts
-- ----------------------------
INSERT INTO `contacts` VALUES ('1', '诸葛亮', '男', '02878893109', '02878893542', '13678032761', '657821', '7068821', '13678032761', '7068821@qq.com', '信息科学与工程学院', '研究方向：数据库与网格计算\n主讲本科生与研究生的《数据库系统》与《数据库课程设计》。');

-- ----------------------------
-- Table structure for continue_education
-- ----------------------------
DROP TABLE IF EXISTS `continue_education`;
CREATE TABLE `continue_education` (
  `number` int(11) NOT NULL AUTO_INCREMENT,
  `code` varchar(20) NOT NULL,
  `begin` date DEFAULT NULL,
  `end` date DEFAULT NULL,
  `mode` varchar(20) DEFAULT NULL,
  `effect` varchar(20) DEFAULT NULL,
  `content` varchar(255) DEFAULT NULL,
  `unit` varchar(50) DEFAULT NULL,
  `location` varchar(100) DEFAULT NULL,
  `cost` varchar(20) DEFAULT NULL,
  `hour` varchar(20) DEFAULT NULL,
  PRIMARY KEY (`number`),
  KEY `code` (`code`),
  CONSTRAINT `continue_education_ibfk_1` FOREIGN KEY (`code`) REFERENCES `staff` (`code`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of continue_education
-- ----------------------------
INSERT INTO `continue_education` VALUES ('1', '3261998001', '2014-05-21', '2014-12-18', '集中', '优秀', '虚拟化网络技术', '华南理工大学', '广东广州', '2000', '48');

-- ----------------------------
-- Table structure for department
-- ----------------------------
DROP TABLE IF EXISTS `department`;
CREATE TABLE `department` (
  `code` varchar(20) NOT NULL,
  `name` varchar(50) NOT NULL,
  `leader` varchar(50) NOT NULL,
  `location` varchar(100) DEFAULT NULL,
  `organization` varchar(255) DEFAULT NULL,
  `teach` varchar(255) DEFAULT NULL,
  `research` varchar(255) DEFAULT NULL,
  `student` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`code`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of department
-- ----------------------------
INSERT INTO `department` VALUES ('1056101', '信息科学与工程学院', '戴经国', '信工楼', '本学院有5个系部：计算机科学与技术系、软件工程系、通信与电子工程系、物联网系、公共教学部。', '本学年的教学工作秩序良好。', '本学年的科研工作较为突出。', '学生工作稳定。');
INSERT INTO `department` VALUES ('1056102', '物理与机电学院', '曹操', '信工楼', '本学院有电子、机械自动化等专业。', '本学年的教学工作秩序较好。', '本学年的科研工作优秀。', '学生工作较好。');

-- ----------------------------
-- Table structure for department_section
-- ----------------------------
DROP TABLE IF EXISTS `department_section`;
CREATE TABLE `department_section` (
  `ID` int(11) NOT NULL AUTO_INCREMENT,
  `division` varchar(50) NOT NULL,
  `description` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of department_section
-- ----------------------------
INSERT INTO `department_section` VALUES ('1', '信息科学与工程学院', '涵盖专业有：计算机科学与技术、软件工程等。');
INSERT INTO `department_section` VALUES ('2', '物理与机电学院', '涵盖专业有：机械自动化、电子等。');
INSERT INTO `department_section` VALUES ('3', '科研处', '');

-- ----------------------------
-- Table structure for discipline_punish
-- ----------------------------
DROP TABLE IF EXISTS `discipline_punish`;
CREATE TABLE `discipline_punish` (
  `number` int(11) NOT NULL AUTO_INCREMENT,
  `code` varchar(20) NOT NULL,
  `sort` varchar(20) DEFAULT NULL,
  `period` varchar(20) DEFAULT NULL,
  `preason` varchar(255) DEFAULT NULL,
  `pdate` date DEFAULT NULL,
  `ddate` date DEFAULT NULL,
  `dreason` varchar(255) DEFAULT NULL,
  `unit` varchar(50) DEFAULT NULL,
  PRIMARY KEY (`number`),
  KEY `code` (`code`),
  CONSTRAINT `discipline_punish_ibfk_1` FOREIGN KEY (`code`) REFERENCES `staff` (`code`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of discipline_punish
-- ----------------------------
INSERT INTO `discipline_punish` VALUES ('1', '3261998002', '通报批评', '3个月', '重大会议迟到', '2017-04-16', '2017-05-16', '错误认识清晰，态度良好，予以解除。', '校长办公室');

-- ----------------------------
-- Table structure for education_degree
-- ----------------------------
DROP TABLE IF EXISTS `education_degree`;
CREATE TABLE `education_degree` (
  `ID` int(11) NOT NULL AUTO_INCREMENT,
  `degree` varchar(50) NOT NULL,
  `description` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of education_degree
-- ----------------------------
INSERT INTO `education_degree` VALUES ('1', '博士', '双证博士学位');
INSERT INTO `education_degree` VALUES ('3', '硕士', '在读博士');

-- ----------------------------
-- Table structure for family_member
-- ----------------------------
DROP TABLE IF EXISTS `family_member`;
CREATE TABLE `family_member` (
  `number` int(11) NOT NULL AUTO_INCREMENT,
  `code` varchar(20) NOT NULL,
  `name` varchar(50) NOT NULL,
  `identity_card` varchar(20) NOT NULL,
  `relation` varchar(20) DEFAULT NULL,
  `birth` date DEFAULT NULL,
  `company` varchar(50) DEFAULT NULL,
  `phone` varchar(20) DEFAULT NULL,
  `duty` varchar(20) DEFAULT NULL,
  `political` varchar(20) DEFAULT NULL,
  `education` varchar(20) DEFAULT NULL,
  PRIMARY KEY (`number`),
  KEY `code` (`code`),
  CONSTRAINT `family_member_ibfk_1` FOREIGN KEY (`code`) REFERENCES `staff` (`code`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of family_member
-- ----------------------------
INSERT INTO `family_member` VALUES ('1', '3261998001', '孙夫人', '470802199411028931', '夫妻', '1994-11-20', '四川省省委组织部', '0208757632', '部长', '共产党', '博士');
INSERT INTO `family_member` VALUES ('2', '3261998001', '聂坤', '440204199511062254', '弟弟', '1995-11-06', '惠州学院', '10086', '', '共产党', '本科');
INSERT INTO `family_member` VALUES ('3', '3261998001', '周昊炜', '440204199605273596', '哥哥', '1996-05-27', '韶关学院', '10000', '处长', '共产党', '硕士');

-- ----------------------------
-- Table structure for fulltime
-- ----------------------------
DROP TABLE IF EXISTS `fulltime`;
CREATE TABLE `fulltime` (
  `code` varchar(20) NOT NULL,
  `name` varchar(50) NOT NULL,
  `leader` varchar(50) NOT NULL,
  `location` varchar(100) DEFAULT NULL,
  `organization` varchar(255) DEFAULT NULL,
  `fruit` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`code`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of fulltime
-- ----------------------------
INSERT INTO `fulltime` VALUES ('1056161', '新兴产业研究院', '张鲁', '行政楼', '本机构有3个科室：产业科、财务科、办公室。', '2016年度本机构取得了令人瞩目的科研业绩。');

-- ----------------------------
-- Table structure for meeting_record
-- ----------------------------
DROP TABLE IF EXISTS `meeting_record`;
CREATE TABLE `meeting_record` (
  `number` int(11) NOT NULL AUTO_INCREMENT,
  `mDate` date DEFAULT NULL,
  `location` varchar(100) DEFAULT NULL,
  `sort` varchar(20) DEFAULT NULL,
  `subject` varchar(100) DEFAULT NULL,
  `host_unit` varchar(50) DEFAULT NULL,
  `participate_unit` varchar(50) DEFAULT NULL,
  `host` varchar(50) DEFAULT NULL,
  `writer` varchar(50) DEFAULT NULL,
  `content` varchar(255) DEFAULT NULL,
  `remark` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`number`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of meeting_record
-- ----------------------------
INSERT INTO `meeting_record` VALUES ('1', '2016-05-24', '图书馆学术报告厅', '学术讲座', '形式语义学与范畴论方法', '信息科学与工程学院', '全校部分相关单位', '诸葛亮', '曹操', '形式语义学blablablabla', '备注！这是备注！！');

-- ----------------------------
-- Table structure for nation_information
-- ----------------------------
DROP TABLE IF EXISTS `nation_information`;
CREATE TABLE `nation_information` (
  `ID` int(11) NOT NULL AUTO_INCREMENT,
  `folk` varchar(50) NOT NULL,
  `description` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=5 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of nation_information
-- ----------------------------
INSERT INTO `nation_information` VALUES ('1', '汉族', '中华民族最多的民族');
INSERT INTO `nation_information` VALUES ('2', '傣族', null);
INSERT INTO `nation_information` VALUES ('3', '蒙古族', null);
INSERT INTO `nation_information` VALUES ('4', '瑶族', '');

-- ----------------------------
-- Table structure for notes
-- ----------------------------
DROP TABLE IF EXISTS `notes`;
CREATE TABLE `notes` (
  `number` int(11) NOT NULL AUTO_INCREMENT,
  `title` varchar(100) DEFAULT NULL,
  `rDate` date DEFAULT NULL,
  `type` varchar(20) DEFAULT NULL,
  `writer` varchar(50) DEFAULT NULL,
  `wUnit` varchar(50) DEFAULT NULL,
  `content` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`number`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of notes
-- ----------------------------
INSERT INTO `notes` VALUES ('1', '本学年度第16周教学工作布置', '2017-06-07', '例会', '郭嘉', '信息科学与工程学院', '曹院长对本周教学工作做了详尽部署，并提前安排了下学期的教学主要工作。');
INSERT INTO `notes` VALUES ('2', '测试安排', '2017-06-08', '例会', '曹操', '物理与机电学院', '测试测试测试测试测试测试测试测试测试测试测试测试测试测试测试测试测试');

-- ----------------------------
-- Table structure for paper
-- ----------------------------
DROP TABLE IF EXISTS `paper`;
CREATE TABLE `paper` (
  `number` int(11) NOT NULL AUTO_INCREMENT,
  `code` varchar(20) NOT NULL,
  `title` varchar(100) DEFAULT NULL,
  `rank` varchar(50) DEFAULT NULL,
  `publication` varchar(100) DEFAULT NULL,
  `pub_time` date DEFAULT NULL,
  `pub_year` varchar(20) DEFAULT NULL,
  `volume` varchar(20) DEFAULT NULL,
  `issue` varchar(20) DEFAULT NULL,
  `page` varchar(10) DEFAULT NULL,
  `level` varchar(20) DEFAULT NULL,
  `type` varchar(20) DEFAULT NULL,
  `index` varchar(20) DEFAULT NULL,
  PRIMARY KEY (`number`),
  KEY `code` (`code`),
  CONSTRAINT `paper_ibfk_1` FOREIGN KEY (`code`) REFERENCES `staff` (`code`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of paper
-- ----------------------------
INSERT INTO `paper` VALUES ('1', '3261998001', '物理低温', '独立', '物理低温专刊', '2017-04-21', '2017', '1', '1', '20', 'SCI期刊', 'CA', 'SCI');

-- ----------------------------
-- Table structure for political_status
-- ----------------------------
DROP TABLE IF EXISTS `political_status`;
CREATE TABLE `political_status` (
  `ID` int(11) NOT NULL AUTO_INCREMENT,
  `party` varchar(50) NOT NULL,
  `description` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of political_status
-- ----------------------------
INSERT INTO `political_status` VALUES ('1', '共产党', '中国共产党成立于1921年。');
INSERT INTO `political_status` VALUES ('2', '共和党-美国', '美国的共和党派。');

-- ----------------------------
-- Table structure for position_information
-- ----------------------------
DROP TABLE IF EXISTS `position_information`;
CREATE TABLE `position_information` (
  `ID` int(11) NOT NULL AUTO_INCREMENT,
  `post` varchar(50) NOT NULL,
  `description` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=6 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of position_information
-- ----------------------------
INSERT INTO `position_information` VALUES ('1', '副教授六级', '副教授第二档');
INSERT INTO `position_information` VALUES ('2', '讲师', null);
INSERT INTO `position_information` VALUES ('3', '实验师', null);
INSERT INTO `position_information` VALUES ('4', '校长', '韶关学院校长');
INSERT INTO `position_information` VALUES ('5', '行政干部', '');

-- ----------------------------
-- Table structure for project
-- ----------------------------
DROP TABLE IF EXISTS `project`;
CREATE TABLE `project` (
  `number` int(11) NOT NULL AUTO_INCREMENT,
  `code` varchar(20) NOT NULL,
  `title` varchar(100) DEFAULT NULL,
  `type` varchar(20) DEFAULT NULL,
  `sort` varchar(20) DEFAULT NULL,
  `level` varchar(20) DEFAULT NULL,
  `rate` varchar(20) DEFAULT NULL,
  `state` varchar(20) DEFAULT NULL,
  `host` varchar(50) DEFAULT NULL,
  `rank` varchar(50) DEFAULT NULL,
  `first` varchar(50) DEFAULT NULL,
  `release` date DEFAULT NULL,
  `unit` varchar(50) DEFAULT NULL,
  `cost` varchar(20) DEFAULT NULL,
  `limit` varchar(20) DEFAULT NULL,
  `begin` date DEFAULT NULL,
  `end` date DEFAULT NULL,
  PRIMARY KEY (`number`),
  KEY `code` (`code`),
  CONSTRAINT `project_ibfk_1` FOREIGN KEY (`code`) REFERENCES `staff` (`code`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of project
-- ----------------------------
INSERT INTO `project` VALUES ('1', '3261998002', '校园网站改造', '科研', '自然科学', '校级', '重点', '在研', '刘备', '网园', '15', '2017-01-21', '校长办', '2000', '1年', '2017-01-27', '2018-01-16');

-- ----------------------------
-- Table structure for reward_record
-- ----------------------------
DROP TABLE IF EXISTS `reward_record`;
CREATE TABLE `reward_record` (
  `number` int(11) NOT NULL AUTO_INCREMENT,
  `code` varchar(20) NOT NULL,
  `title` varchar(50) DEFAULT NULL,
  `sort` varchar(20) DEFAULT NULL,
  `level` varchar(20) DEFAULT NULL,
  `unit` varchar(50) DEFAULT NULL,
  `category` varchar(20) DEFAULT NULL,
  `date` date DEFAULT NULL,
  PRIMARY KEY (`number`),
  KEY `code` (`code`),
  CONSTRAINT `reward_record_ibfk_1` FOREIGN KEY (`code`) REFERENCES `staff` (`code`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of reward_record
-- ----------------------------
INSERT INTO `reward_record` VALUES ('1', '3261998001', '中国大学生计算机设计大赛·微电影组', '国家级', '三等奖', '中国大学生计算机设计大赛组委会', '技术开发', '2016-08-10');

-- ----------------------------
-- Table structure for section
-- ----------------------------
DROP TABLE IF EXISTS `section`;
CREATE TABLE `section` (
  `code` varchar(20) NOT NULL,
  `name` varchar(50) NOT NULL,
  `leader` varchar(50) NOT NULL,
  `location` varchar(100) DEFAULT NULL,
  `organization` varchar(255) DEFAULT NULL,
  `rule` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`code`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of section
-- ----------------------------
INSERT INTO `section` VALUES ('1056131', '科研处', '刘备', '行政楼五楼', '本部门有4个科室：纵向项目管理科、横向项目管理科、成果统计科、办公室。', '坚决拥护学校领导的各种政策方针。');

-- ----------------------------
-- Table structure for staff
-- ----------------------------
DROP TABLE IF EXISTS `staff`;
CREATE TABLE `staff` (
  `code` varchar(20) NOT NULL,
  `name` varchar(50) NOT NULL,
  `sex` varchar(10) DEFAULT NULL,
  `birth` date DEFAULT NULL,
  `marriage` varchar(10) DEFAULT NULL,
  `age` int(4) DEFAULT NULL,
  `party` varchar(20) DEFAULT NULL,
  `race` varchar(20) DEFAULT NULL,
  `identity_card` varchar(20) DEFAULT NULL,
  `work_date` date DEFAULT NULL,
  `post` varchar(20) DEFAULT NULL,
  `native` varchar(20) DEFAULT NULL,
  `entry_date` date DEFAULT NULL,
  `salary` varchar(20) DEFAULT NULL,
  `degree` varchar(20) DEFAULT NULL,
  `department` varchar(50) DEFAULT NULL,
  `technical` varchar(20) DEFAULT NULL,
  `job` varchar(20) DEFAULT NULL,
  `office_phone` varchar(20) DEFAULT NULL,
  `mobile_phone` varchar(20) DEFAULT NULL,
  `email` varchar(50) DEFAULT NULL,
  `university` varchar(50) DEFAULT NULL,
  `subject` varchar(50) DEFAULT NULL,
  `graduate_date` date DEFAULT NULL,
  `address` varchar(100) DEFAULT NULL,
  PRIMARY KEY (`code`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of staff
-- ----------------------------
INSERT INTO `staff` VALUES ('3261998001', '刘备', '男', '1975-01-01', '已婚', '41', '共产党', '汉族', '320804197501012348', '1995-01-01', '行政干部', '河北涿州', '2014-01-01', '教授二级', '博士', '科研处', '教授', '处长', '02846789392', '15924570982', 'ycsgu@sact.edu.cn', '中国科技大学', '人事管理', '1995-01-01', '四川省成都市双流区');
INSERT INTO `staff` VALUES ('3261998002', '诸葛亮', '男', '1969-07-08', '已婚', '50', '共和党-美国', '汉族', '', '2017-05-11', '校长', '', '2011-04-17', '教授二级', '硕士', '物理与机电学院', '教授', '副处长', '', '', '', '', '', '1999-02-11', '');

-- ----------------------------
-- Table structure for technical_assess
-- ----------------------------
DROP TABLE IF EXISTS `technical_assess`;
CREATE TABLE `technical_assess` (
  `number` int(11) NOT NULL AUTO_INCREMENT,
  `code` varchar(20) NOT NULL,
  `title` varchar(100) DEFAULT NULL,
  `mode` varchar(20) DEFAULT NULL,
  `unit` varchar(50) DEFAULT NULL,
  `assess_date` date DEFAULT NULL,
  `hire` varchar(20) DEFAULT NULL,
  `begin` date DEFAULT NULL,
  `end` date DEFAULT NULL,
  `employ` varchar(50) DEFAULT NULL,
  PRIMARY KEY (`number`),
  KEY `code` (`code`),
  CONSTRAINT `technical_assess_ibfk_1` FOREIGN KEY (`code`) REFERENCES `staff` (`code`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of technical_assess
-- ----------------------------
INSERT INTO `technical_assess` VALUES ('1', '3261998001', '广东省一级教师', '评审', '广东省教育局', '2016-11-25', '教授', '2017-01-14', '2018-12-21', '韶关学院');

-- ----------------------------
-- Table structure for technical_post
-- ----------------------------
DROP TABLE IF EXISTS `technical_post`;
CREATE TABLE `technical_post` (
  `ID` int(11) NOT NULL AUTO_INCREMENT,
  `title` varchar(50) NOT NULL,
  `description` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of technical_post
-- ----------------------------
INSERT INTO `technical_post` VALUES ('1', '副教授', '副教授共有三档：七级、六级与五级。');
INSERT INTO `technical_post` VALUES ('2', '教授', '');

-- ----------------------------
-- Table structure for user
-- ----------------------------
DROP TABLE IF EXISTS `user`;
CREATE TABLE `user` (
  `customer` varchar(50) NOT NULL,
  `password` varchar(50) NOT NULL,
  `privilege` tinyint(1) NOT NULL,
  PRIMARY KEY (`customer`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of user
-- ----------------------------
INSERT INTO `user` VALUES ('admin', 'admin', '1');
INSERT INTO `user` VALUES ('test', 'test', '0');

-- ----------------------------
-- Table structure for wage_level
-- ----------------------------
DROP TABLE IF EXISTS `wage_level`;
CREATE TABLE `wage_level` (
  `ID` int(11) NOT NULL AUTO_INCREMENT,
  `salary` varchar(50) NOT NULL,
  `description` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of wage_level
-- ----------------------------
INSERT INTO `wage_level` VALUES ('1', '副教授六级', '副教授工资等级中第二档。');
INSERT INTO `wage_level` VALUES ('2', '教授二级', '');

-- ----------------------------
-- Table structure for work_experience
-- ----------------------------
DROP TABLE IF EXISTS `work_experience`;
CREATE TABLE `work_experience` (
  `number` int(11) NOT NULL AUTO_INCREMENT,
  `code` varchar(20) NOT NULL,
  `begin_date` date DEFAULT NULL,
  `end_date` date DEFAULT NULL,
  `company` varchar(50) DEFAULT NULL,
  `department` varchar(50) DEFAULT NULL,
  `duty` varchar(20) DEFAULT NULL,
  `technical` varchar(20) DEFAULT NULL,
  `education` varchar(20) DEFAULT NULL,
  `location` varchar(100) DEFAULT NULL,
  `proof` varchar(50) DEFAULT NULL,
  PRIMARY KEY (`number`),
  KEY `code` (`code`),
  CONSTRAINT `work_experience_ibfk_1` FOREIGN KEY (`code`) REFERENCES `staff` (`code`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of work_experience
-- ----------------------------
INSERT INTO `work_experience` VALUES ('1', '3261998001', '2017-01-15', '2017-04-12', '韶关学院', '科研处', '处长', '正处级', '博士', '韶关市', '诸葛亮');
